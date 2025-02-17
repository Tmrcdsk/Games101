//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Vector3f L_dir;
    Vector3f L_indir;

    // 从像素发出的光线与物体的交点
    Intersection obj_inter = intersect(ray);
    if (!obj_inter.happened)
        return L_dir;
    
    // 打到光源
    if (obj_inter.m->hasEmission())
        return obj_inter.m->getEmission();

    // 打到物体
    Vector3f p = obj_inter.coords;
    Vector3f wo = ray.direction;

    Material* m = obj_inter.m;
    Vector3f N = obj_inter.normal.normalized();
    
    Intersection light_inter;
    float pdf_light;
    sampleLight(light_inter, pdf_light);
    // Get x, ws, NN, emit from inter
    Vector3f x = light_inter.coords;
    Vector3f ws = (x - p).normalized();
    Vector3f NN = light_inter.normal.normalized();
    Vector3f emit = light_inter.emit;
    float d = (x - p).norm();

    // Shoot a ray from p to x
    Ray obj2light(p, ws);
    float dist = intersect(obj2light).distance;
    // If the ray is not blocked in the middle
    if (fabs(dist - d) < 0.001) { 
        // 此处dist一定小于d，因此也可以改为 `if (dist - d > -0.001)`
        L_dir = emit * m->eval(wo, ws, N) * dotProduct(ws, N) * dotProduct(-ws, NN) / std::pow(d, 2) / pdf_light;
    }

    // Test Russian Roulette with probability RussianRoulette
    float P_RR = get_random_float();
    if (P_RR < RussianRoulette) {
        Vector3f wi = m->sample(wo, N).normalized();
        Ray r(p, wi);
        Intersection inter = intersect(r);
        if (inter.happened && !inter.m->hasEmission()) {
            L_indir = castRay(r, depth + 1) * m->eval(wo, wi, N) * dotProduct(wi, N) / m->pdf(wo, wi, N) / RussianRoulette;
        }
    }

    return L_dir + L_indir;
}