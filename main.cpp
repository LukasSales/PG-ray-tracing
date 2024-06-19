#include <iostream>
#include <vector>
#include "camera.h"
#include "sphere.h"
#include "plane.h"
#include "ray.h"
#include "lodepng.h"

void render(const Camera& camera, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes) {
    Vec3 u, v, w;
    camera.getCameraBasis(u, v, w);

    std::vector<unsigned char> image;
    image.resize(camera.hres * camera.vres * 4);

    for (int j = 0; j < camera.vres; ++j) {
        for (int i = 0; i < camera.hres; ++i) {
            double x = (2 * ((i + 0.5) / camera.hres) - 1);
            double y = (1 - 2 * ((j + 0.5) / camera.vres));

            Vec3 direction = (u * x + v * y - w * camera.distance).normalize();
            Ray ray(Point3(camera.position.x, camera.position.y, camera.position.z), direction);

            Vec3 color(0, 0, 0);
            double t = 1e6;

            for (const auto& sphere : spheres) {
                double t_sphere;
                if (sphere.intersect(ray, t_sphere) && t_sphere < t) {
                    t = t_sphere;
                    color = sphere.color;
                }
            }

            for (const auto& plane : planes) {
                double t_plane;
                if (plane.intersect(ray, t_plane) && t_plane < t) {
                    t = t_plane;
                    color = plane.color;
                }
            }

            image[4 * (j * camera.hres + i) + 0] = static_cast<unsigned char>(color.x * 255);
            image[4 * (j * camera.hres + i) + 1] = static_cast<unsigned char>(color.y * 255);
            image[4 * (j * camera.hres + i) + 2] = static_cast<unsigned char>(color.z * 255);
            image[4 * (j * camera.hres + i) + 3] = 255;
        }
    }

    unsigned error = lodepng::encode("output.png", image, camera.hres, camera.vres);
        if (error) {
            std::cout << "Encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }

}

int main() {
    Point3 cameraPosition(0, 0, 0);
    Point3 lookAt(0, 0, -1);
    Vec3 up(0, 1, 0);
    double distance = 1.0;
    int vres = 500;
    int hres = 500;

    Camera camera(cameraPosition, lookAt, up, distance, vres, hres);

    std::vector<Sphere> spheres = {
        Sphere(Point3(0, 0, -3), 1, Vec3(0, 0, 1)),
        Sphere(Point3(1, 0, -3.5), 0.5, Vec3(1, 1, 1)),
        Sphere(Point3(1, 0, -2), 0.25, Vec3(0, 1, 1)),
        Sphere(Point3(-1.5, -0.5, -2.5), 0.6, Vec3(0, 0, 0)),
        Sphere(Point3(0.8, -0.9, -3), 0.8, Vec3(0, 1, 0)),
        Sphere(Point3(0.5, -0.5, -1.5), 0.28, Vec3(1, 1, 0)),
        Sphere(Point3(1.5, 2, -3), 0.58, Vec3(0, 1, 1)),
        Sphere(Point3(2.5, 1, -5), 0.48, Vec3(1, 0, 0))

    };

    std::vector<Plane> planes = {
    Plane(Point3(0, -1, 0), Vec3(0, 1, 0), Vec3(1, 0, 1))// Plano com cor roxa
    };

    render(camera, spheres, planes);

    return 0;
}
