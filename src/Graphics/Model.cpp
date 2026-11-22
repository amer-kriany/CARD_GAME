#include "Model.h"

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
    // The pose of one node: translation, rotation, scale.
    struct NodePose {
        glm::vec3 translation{0.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f};

        bool useMatrix = false;
        glm::mat4 matrix{1.0f};

        glm::mat4 local() const
        {
            if (useMatrix)
            {
                return matrix;
            }

            return glm::translate(glm::mat4(1.0f), translation)
                 * glm::mat4_cast(rotation)
                 * glm::scale(glm::mat4(1.0f), scale);
        }
    };

    size_t nodeIndex(const cgltf_data* data, const cgltf_node* node)
    {
        return static_cast<size_t>(node - data->nodes);
    }

    // Value of an animation track at a given time (linear interpolation).
    // "components" is 3 for translation/scale, 4 for rotation.
    glm::vec4 sampleTrack(
        const cgltf_animation_sampler& sampler,
        int components,
        float time
    )
    {
        const cgltf_accessor* times = sampler.input;
        const cgltf_accessor* values = sampler.output;

        auto readValue = [&](size_t index)
        {
            float v[4] = {0.0f, 0.0f, 0.0f, 1.0f};
            cgltf_accessor_read_float(values, index, v, components);
            return glm::vec4(v[0], v[1], v[2], v[3]);
        };

        auto readTime = [&](size_t index)
        {
            float t = 0.0f;
            cgltf_accessor_read_float(times, index, &t, 1);
            return t;
        };

        const size_t count = times->count;

        // Before the first key or after the last key: hold that value.
        if (time <= readTime(0))
        {
            return readValue(0);
        }

        if (time >= readTime(count - 1))
        {
            return readValue(count - 1);
        }

        // Find the two keys around "time" and blend between them.
        size_t next = 1;
        while (next < count && readTime(next) < time)
        {
            ++next;
        }

        const size_t previous = next - 1;
        const float t0 = readTime(previous);
        const float t1 = readTime(next);
        const float blend = (time - t0) / (t1 - t0);

        const glm::vec4 a = readValue(previous);
        const glm::vec4 b = readValue(next);

        if (components == 4)
        {
            // Rotations are quaternions: blend them with slerp.
            const glm::quat qa(a.w, a.x, a.y, a.z);
            const glm::quat qb(b.w, b.x, b.y, b.z);
            const glm::quat q = glm::slerp(qa, qb, blend);

            return glm::vec4(q.x, q.y, q.z, q.w);
        }

        return glm::mix(a, b, blend);
    }
}

Model::Model(
    const std::string& path,
    const std::string& animationName,
    float animationTime
)
{
    cgltf_options options = {};
    cgltf_data* data = nullptr;

    if (cgltf_parse_file(&options, path.c_str(), &data) != cgltf_result_success)
    {
        std::cerr << "Cannot read model: " << path << "\n";
        return;
    }

    if (cgltf_load_buffers(&options, data, path.c_str()) != cgltf_result_success)
    {
        std::cerr << "Cannot load model data: " << path << "\n";
        cgltf_free(data);
        return;
    }

    // ---- 1. Start from every node's rest pose ----
    std::vector<NodePose> poses(data->nodes_count);

    for (size_t i = 0; i < data->nodes_count; ++i)
    {
        const cgltf_node& node = data->nodes[i];
        NodePose& pose = poses[i];

        if (node.has_matrix)
        {
            pose.useMatrix = true;
            pose.matrix = glm::make_mat4(node.matrix);
            continue;
        }

        if (node.has_translation)
        {
            pose.translation = glm::make_vec3(node.translation);
        }

        if (node.has_rotation)
        {
            pose.rotation = glm::quat(
                node.rotation[3],
                node.rotation[0],
                node.rotation[1],
                node.rotation[2]
            );
        }

        if (node.has_scale)
        {
            pose.scale = glm::make_vec3(node.scale);
        }
    }

    // ---- 2. Freeze the chosen animation at animationTime ----
    for (size_t a = 0; a < data->animations_count; ++a)
    {
        const cgltf_animation& animation = data->animations[a];

        if (animationName.empty() || !animation.name
            || animationName != animation.name)
        {
            continue;
        }

        for (size_t c = 0; c < animation.channels_count; ++c)
        {
            const cgltf_animation_channel& channel = animation.channels[c];

            if (!channel.target_node)
            {
                continue;
            }

            NodePose& pose = poses[nodeIndex(data, channel.target_node)];

            switch (channel.target_path)
            {
                case cgltf_animation_path_type_translation:
                {
                    glm::vec4 v = sampleTrack(*channel.sampler, 3, animationTime);
                    pose.translation = glm::vec3(v);
                    break;
                }
                case cgltf_animation_path_type_rotation:
                {
                    glm::vec4 v = sampleTrack(*channel.sampler, 4, animationTime);
                    pose.rotation = glm::quat(v.w, v.x, v.y, v.z);
                    break;
                }
                case cgltf_animation_path_type_scale:
                {
                    glm::vec4 v = sampleTrack(*channel.sampler, 3, animationTime);
                    pose.scale = glm::vec3(v);
                    break;
                }
                default:
                    break;
            }
        }

        break;
    }

    // ---- 3. World matrix of every node (parent * local) ----
    std::vector<glm::mat4> world(data->nodes_count, glm::mat4(1.0f));
    std::vector<bool> done(data->nodes_count, false);

    // Walks up to the parent first, so the parent's matrix is ready.
    auto computeWorld = [&](auto&& self, const cgltf_node* node) -> glm::mat4
    {
        const size_t index = nodeIndex(data, node);

        if (done[index])
        {
            return world[index];
        }

        glm::mat4 result = poses[index].local();

        if (node->parent)
        {
            result = self(self, node->parent) * result;
        }

        world[index] = result;
        done[index] = true;

        return result;
    };

    // ---- 4. Pose every mesh and collect triangles ----
    struct RawPart {
        std::vector<float> vertices;   // x y z u v nx ny nz
        std::vector<unsigned int> indices;
        glm::vec3 color{1.0f};
    };

    std::vector<RawPart> rawParts;

    glm::vec3 minCorner(std::numeric_limits<float>::max());
    glm::vec3 maxCorner(std::numeric_limits<float>::lowest());

    for (size_t n = 0; n < data->nodes_count; ++n)
    {
        const cgltf_node& node = data->nodes[n];

        if (!node.mesh)
        {
            continue;
        }

        const glm::mat4 nodeWorld = computeWorld(computeWorld, &node);

        // Skinning matrix of every bone: where the bone is now,
        // times where it was when the mesh was attached to it.
        std::vector<glm::mat4> boneMatrices;

        if (node.skin)
        {
            for (size_t j = 0; j < node.skin->joints_count; ++j)
            {
                glm::mat4 inverseBind(1.0f);

                if (node.skin->inverse_bind_matrices)
                {
                    float m[16];
                    cgltf_accessor_read_float(
                        node.skin->inverse_bind_matrices, j, m, 16
                    );
                    inverseBind = glm::make_mat4(m);
                }

                boneMatrices.push_back(
                    computeWorld(computeWorld, node.skin->joints[j])
                    * inverseBind
                );
            }
        }

        for (size_t p = 0; p < node.mesh->primitives_count; ++p)
        {
            const cgltf_primitive& primitive = node.mesh->primitives[p];

            if (primitive.type != cgltf_primitive_type_triangles
                || !primitive.indices)
            {
                continue;
            }

            const cgltf_accessor* positions = nullptr;
            const cgltf_accessor* normals = nullptr;
            const cgltf_accessor* joints = nullptr;
            const cgltf_accessor* weights = nullptr;

            for (size_t a = 0; a < primitive.attributes_count; ++a)
            {
                const cgltf_attribute& attribute = primitive.attributes[a];

                if (attribute.index != 0)
                {
                    continue;
                }

                if (attribute.type == cgltf_attribute_type_position)
                    positions = attribute.data;
                else if (attribute.type == cgltf_attribute_type_normal)
                    normals = attribute.data;
                else if (attribute.type == cgltf_attribute_type_joints)
                    joints = attribute.data;
                else if (attribute.type == cgltf_attribute_type_weights)
                    weights = attribute.data;
            }

            if (!positions)
            {
                continue;
            }

            const bool skinned =
                joints && weights && !boneMatrices.empty();

            RawPart part;

            // Material color. glTF stores it in linear space, our shader
            // writes it straight to the screen, so convert to sRGB-ish.
            if (primitive.material
                && primitive.material->has_pbr_metallic_roughness)
            {
                const float* c =
                    primitive.material->pbr_metallic_roughness.base_color_factor;

                part.color = glm::vec3(
                    std::pow(c[0], 1.0f / 2.2f),
                    std::pow(c[1], 1.0f / 2.2f),
                    std::pow(c[2], 1.0f / 2.2f)
                );
            }

            for (size_t v = 0; v < positions->count; ++v)
            {
                float pos[3] = {0, 0, 0};
                float nrm[3] = {0, 1, 0};

                cgltf_accessor_read_float(positions, v, pos, 3);

                if (normals)
                {
                    cgltf_accessor_read_float(normals, v, nrm, 3);
                }

                glm::mat4 matrix = nodeWorld;

                if (skinned)
                {
                    cgltf_uint j[4] = {0, 0, 0, 0};
                    float w[4] = {0, 0, 0, 0};

                    cgltf_accessor_read_uint(joints, v, j, 4);
                    cgltf_accessor_read_float(weights, v, w, 4);

                    // Blend the 4 bones that influence this vertex.
                    glm::mat4 blended(0.0f);

                    for (int k = 0; k < 4; ++k)
                    {
                        blended += w[k] * boneMatrices[j[k]];
                    }

                    matrix = blended;
                }

                const glm::vec3 position =
                    glm::vec3(matrix * glm::vec4(pos[0], pos[1], pos[2], 1.0f));

                const glm::vec3 normal = glm::normalize(
                    glm::vec3(matrix * glm::vec4(nrm[0], nrm[1], nrm[2], 0.0f))
                );

                minCorner = glm::min(minCorner, position);
                maxCorner = glm::max(maxCorner, position);

                const float vertex[8] = {
                    position.x, position.y, position.z,
                    0.0f, 0.0f,
                    normal.x, normal.y, normal.z
                };

                part.vertices.insert(
                    part.vertices.end(), vertex, vertex + 8
                );
            }

            for (size_t i = 0; i < primitive.indices->count; ++i)
            {
                part.indices.push_back(
                    static_cast<unsigned int>(
                        cgltf_accessor_read_index(primitive.indices, i)
                    )
                );
            }

            rawParts.push_back(std::move(part));
        }
    }

    cgltf_free(data);

    if (rawParts.empty())
    {
        std::cerr << "Model has no triangles: " << path << "\n";
        return;
    }

    // ---- 5. Move the feet to the origin, then upload to the GPU ----
    // x and z: center of the model. y: the lowest point.
    const glm::vec3 shift(
        -(minCorner.x + maxCorner.x) * 0.5f,
        -minCorner.y,
        -(minCorner.z + maxCorner.z) * 0.5f
    );

    height_ = maxCorner.y - minCorner.y;

    for (RawPart& raw : rawParts)
    {
        for (size_t i = 0; i < raw.vertices.size(); i += 8)
        {
            raw.vertices[i + 0] += shift.x;
            raw.vertices[i + 1] += shift.y;
            raw.vertices[i + 2] += shift.z;
        }

        Part part;
        part.color = raw.color;
        part.indexCount = static_cast<GLsizei>(raw.indices.size());

        glGenVertexArrays(1, &part.vao);
        glGenBuffers(1, &part.vbo);
        glGenBuffers(1, &part.ebo);

        glBindVertexArray(part.vao);

        glBindBuffer(GL_ARRAY_BUFFER, part.vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            raw.vertices.size() * sizeof(float),
            raw.vertices.data(),
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part.ebo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            raw.indices.size() * sizeof(unsigned int),
            raw.indices.data(),
            GL_STATIC_DRAW
        );

        // Same layout as Cube: position, UV, normal.
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        parts_.push_back(part);
    }
}

Model::~Model()
{
    for (const Part& part : parts_)
    {
        glDeleteVertexArrays(1, &part.vao);
        glDeleteBuffers(1, &part.vbo);
        glDeleteBuffers(1, &part.ebo);
    }
}

void Model::draw(const Shader& shader, const glm::mat4& placement) const
{
    shader.use();

    // Material 6 = solid color with simple lighting (see basic.frag).
    shader.setInt("useTexture", 6);
    shader.setMat4("model", placement);

    for (const Part& part : parts_)
    {
        shader.setVec4("tint", glm::vec4(part.color, 1.0f));

        glBindVertexArray(part.vao);
        glDrawElements(
            GL_TRIANGLES, part.indexCount, GL_UNSIGNED_INT, nullptr
        );
    }

    glBindVertexArray(0);
}