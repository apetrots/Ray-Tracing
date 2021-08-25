#ifndef RT_HITTABLE_LIST
#define RT_HITTABLE_LIST

#include <memory>
#include <vector>

#include "hittable.h"

using std::make_shared;
using std::shared_ptr;
using std::vector;

class hittable_list : public hittable
{
    vector<shared_ptr<hittable>> objs;

public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> _obj) { add(_obj); }

    void clear()
    {
        objs.clear();
    }

    void add(shared_ptr<hittable> obj)
    {
        objs.push_back(obj);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const 
{
    hit_record temp;

    bool hit = false;
    double closest = t_max;

    for (const auto& obj : objs )
    {
        if (obj->hit(r, t_min, closest, temp))
        {
            hit = true;
            closest = temp.time;
            rec = temp;
        }
    }

    return hit;
}

#endif // RT_HITTABLE_LIST