
#pragma once

class Card {
public:
    explicit Card(int id)
        : id_(id)
    {
    }

    int id() const
    {
        return id_;
    }

private:
    int id_;
};
