#define TINYOBJLOADER_IMPLEMENTATION
#include "third_party/tiny_obj_loader.h"

#include "raytracer/bvh.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/triangle_mesh.hpp"

bool triangle_mesh::load_from_obj(const std::string& filepath, std::shared_ptr<material> material) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        std::cerr << "Failed to load OBJ file: " << err << std::endl;
        return false;
    }

    hittable_list triangles;

    // Iterate through shapes and create triangles
    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
            // Get vertex indices
            auto idx0 = shape.mesh.indices[i];
            auto idx1 = shape.mesh.indices[i + 1];
            auto idx2 = shape.mesh.indices[i + 2];

            // Extract positions
            point3 v0(attrib.vertices[3 * idx0.vertex_index],
                      attrib.vertices[3 * idx0.vertex_index + 1],
                      attrib.vertices[3 * idx0.vertex_index + 2]);
            point3 v1(attrib.vertices[3 * idx1.vertex_index],
                      attrib.vertices[3 * idx1.vertex_index + 1],
                      attrib.vertices[3 * idx1.vertex_index + 2]);
            point3 v2(attrib.vertices[3 * idx2.vertex_index],
                      attrib.vertices[3 * idx2.vertex_index + 1],
                      attrib.vertices[3 * idx2.vertex_index + 2]);

            // Create triangle with shared material
            triangles.add(std::make_shared<triangle>(v0, v1 - v0, v2 - v0, material));
        }
    }

    // Build BVH for the triangles
    if (!triangles.objects.empty()) {
        bvh_root = std::make_shared<bvh_node>(triangles);
        bbox = bvh_root->bounding_box();
    }

    return true;
}

bool triangle_mesh::hit(const ray& r, interval ray_t, hit_record& rec) const {
    if (!bvh_root) return false;
    return bvh_root->hit(r, ray_t, rec);
}
