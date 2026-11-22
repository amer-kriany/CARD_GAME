#version 330 core

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec4 tint;
uniform int useTexture;

// Rounded rectangle mask.
float roundedBox(vec2 p, vec2 size, float radius)
{
    vec2 q = abs(p) - size + vec2(radius);

    return length(max(q, 0.0))
        + min(max(q.x, q.y), 0.0)
        - radius;
}

float shapeMask()
{
    vec2 p = TexCoord - vec2(0.5);

    float distanceValue = roundedBox(
        p,
        vec2(0.48),
        0.035
    );

    return 1.0 - smoothstep(
        -0.008,
        0.008,
        distanceValue
    );
}

// Procedural antique gold.
vec4 goldMaterial()
{
    float verticalGradient = smoothstep(
        0.0,
        1.0,
        TexCoord.y
    );

    float shine = 1.0 - abs(TexCoord.y - 0.72);
    shine = smoothstep(0.35, 1.0, shine);

    vec3 darkGold = vec3(0.38, 0.20, 0.055);
    vec3 brightGold = vec3(1.0, 0.82, 0.43);

    vec3 color = mix(
        darkGold,
        brightGold,
        verticalGradient * 0.8
    );

    color += shine * 0.16;

    return vec4(color * tint.rgb, tint.a);
}

// Procedural leather.
vec4 leatherMaterial()
{
    vec2 p = TexCoord * 85.0;

    float grain = sin(p.x * 1.7 + sin(p.y * 0.8))
                * cos(p.y * 1.3);

    float variation = grain * 0.025;

    float edgeShade = smoothstep(
        0.0,
        0.5,
        min(
            min(TexCoord.x, 1.0 - TexCoord.x),
            min(TexCoord.y, 1.0 - TexCoord.y)
        )
    );

    vec3 color = tint.rgb;
    color *= 0.91 + variation;
    color *= 0.88 + edgeShade * 0.12;

    return vec4(color, tint.a);
}

void main()
{
    // Materials 5 and 6 are for the room and its decor: no rounded-corner
    // mask, otherwise a thin strip would be cut from every wall, floor
    // and box.
    float mask = (useTexture == 5 || useTexture == 6) ? 1.0 : shapeMask();

    if (mask < 0.01)
        discard;

    vec4 result;

    if (useTexture == 1 || useTexture == 5)
    {
        result = texture(ourTexture, TexCoord) * tint;
    }
    else if (useTexture == 2)
    {
        vec4 texColor = texture(ourTexture, TexCoord);

        float distanceFromCenter = distance(
            TexCoord,
            vec2(0.5)
        );

        float shade = 1.0 - smoothstep(
            0.25,
            0.72,
            distanceFromCenter
        ) * 0.22;

        result = vec4(
            texColor.rgb * tint.rgb * shade,
            texColor.a * tint.a
        );
    }
    else if (useTexture == 3)
    {
        result = goldMaterial();
    }
    else if (useTexture == 4)
    {
        result = leatherMaterial();
    }
    else if (useTexture == 6)
    {
        // Solid color with simple lighting, so the faces of a box differ.
        vec3 lightDirection = normalize(vec3(0.35, 0.8, 0.5));
        float diffuse = max(dot(normalize(Normal), lightDirection), 0.0);

        result = vec4(tint.rgb * (0.45 + 0.55 * diffuse), tint.a);
    }
    else
    {
        result = tint;
    }

    FragColor = vec4(result.rgb, result.a * mask);
}