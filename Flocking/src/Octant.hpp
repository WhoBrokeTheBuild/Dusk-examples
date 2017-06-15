#ifndef OCTANT_HPP
#define OCTANT_HPP

#include <vector>

template <typename T>
struct OctantNode
{
public:

    OctantNode(glm::vec3 rangeMin, glm::vec3 rangeMax, float size)
    {
        _min = rangeMin;
        _max = rangeMax;
        _size = size;

        glm::vec3 len = _max - _min;
        if (len.x <= _size || len.y <= _size || len.z <= _size)
        {
            _isLeaf = true;
            memset(_children, 0, sizeof(_children));
            return;
        }

        _med = _min + (rangeMax - _min) * 0.5f;

        unsigned short index = 0;
        for (int z = 0; z < 2; ++z)
        {
            for (int y = 0; y < 2; ++y)
            {
                for (int x = 0; x < 2; ++x)
                {
                    _children[index] = new OctantNode(
                        glm::vec3(
                            (x == 0 ? _min.x : _med.x),
                            (y == 0 ? _min.y : _med.y),
                            (z == 0 ? _min.z : _med.z) ),
                        glm::vec3(
                            (x == 0 ? _med.x : _max.x),
                            (y == 0 ? _med.y : _max.y),
                            (z == 0 ? _med.z : _max.z) ),
                        _size
                    );
                    ++index;
                }
            }
        }
    }

    ~OctantNode()
    {
        if (_isLeaf) return;

        for (unsigned short i = 0; i < 8; ++i)
        {
            delete _children[i];
        }
    }

    unsigned int GetIndex(glm::vec3 pos)
    {
        // Make positions [zero, max)
        glm::vec3 tmpMax = (_max - _min);
        glm::ivec3 maxInd = glm::ivec3(tmpMax) / glm::ivec3(_size);
        pos -= _min;

        unsigned int index = ( (int)pos.x / (int)_size) +
                             (((int)pos.y / (int)_size) * maxInd.x) +
                             (((int)pos.z / (int)_size) * maxInd.x * maxInd.y);
        return index;
    }

    void AddMember(glm::vec3 pos, T member)
    {
        if (pos.x < _min.x || pos.x > _max.x ||
            pos.y < _min.y || pos.y > _max.y ||
            pos.z < _min.z || pos.z > _max.z)
        {
            return;
        }

        if (_isLeaf)
        {
            _members.push_back(member);
            return;
        }

        unsigned short index = 0;
        if (pos.x > _med.x)
        {
            index ^= 1;
        }
        if (pos.y > _med.y)
        {
            index ^= 2;
        }
        if (pos.z > _med.z)
        {
            index ^= 4;
        }

        return _children[index]->AddMember(pos, member);
    }

    void RemoveMember(glm::vec3 pos, T member)
    {
        if (pos.x < _min.x || pos.x > _max.x ||
            pos.y < _min.y || pos.y > _max.y ||
            pos.z < _min.z || pos.z > _max.z)
        {
            return;
        }

        if (_isLeaf)
        {
            _members.erase(std::remove(_members.begin(), _members.end(), member), _members.end());
            return;
        }

        unsigned short index = 0;
        if (pos.x > _med.x)
        {
            index ^= 1;
        }
        if (pos.y > _med.y)
        {
            index ^= 2;
        }
        if (pos.z > _med.z)
        {
            index ^= 4;
        }

        return _children[index]->RemoveMember(pos, member);
    }

    void ClearMembers()
    {
        if (_isLeaf)
        {
            _members.clear();
            return;
        }

        for (unsigned short i = 0; i < 8; ++i)
        {
            _children[i]->ClearMembers();
        }
    }

    std::vector<T> GetMembers(glm::vec3 pos)
    {
        if (pos.x < _min.x || pos.x > _max.x ||
            pos.y < _min.y || pos.y > _max.y ||
            pos.z < _min.z || pos.z > _max.z)
        {
            return std::vector<T>();
        }

        if (_isLeaf)
        {
            return _members;
        }

        unsigned short index = 0;
        if (pos.x > _med.x)
        {
            index ^= 1;
        }
        if (pos.y > _med.y)
        {
            index ^= 2;
        }
        if (pos.z > _med.z)
        {
            index ^= 4;
        }

        return _children[index]->GetMembers(pos);
    }

    std::vector<T> GetAdjacentMembers(glm::vec3 pos)
    {
        std::vector<T> members = GetMembers(pos);

        const std::vector<T>& vector1 = GetMembers(pos + glm::vec3(_size, 0, 0));
        const std::vector<T>& vector2 = GetMembers(pos + glm::vec3(0, _size, 0));
        const std::vector<T>& vector3 = GetMembers(pos + glm::vec3(0, 0, _size));
        const std::vector<T>& vector4 = GetMembers(pos - glm::vec3(_size, 0, 0));
        const std::vector<T>& vector5 = GetMembers(pos - glm::vec3(0, _size, 0));
        const std::vector<T>& vector6 = GetMembers(pos - glm::vec3(0, 0, _size));

        members.insert(members.end(), vector1.begin(), vector1.end());
        members.insert(members.end(), vector2.begin(), vector2.end());
        members.insert(members.end(), vector3.begin(), vector3.end());
        members.insert(members.end(), vector4.begin(), vector4.end());
        members.insert(members.end(), vector5.begin(), vector5.end());
        members.insert(members.end(), vector6.begin(), vector6.end());

        return members;
    }

private:
    glm::vec3  _min;
    glm::vec3  _max;
    glm::vec3  _med;

    float _size;

    bool _isLeaf = false;

    OctantNode<T> * _children[8];
    std::vector<T> _members;

}; // struct OctantNode

#endif // OCTANT_HPP
