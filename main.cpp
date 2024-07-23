#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "intersection.h"
#include "camera.h"
#include "lodepng.h"
<<<<<<< Updated upstream
#include <fstream>

bool findClosestIntersection(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, Intersection& closestIntersection) {
=======
#include "mesh.h"
#include "matrix4.h"
#include "transformation.h"
#include <fstream>

#define M_PI 3.14159265358979323846 

// Função para encontrar a interseção mais próxima entre um raio e uma lista de objetos
bool findClosestIntersection(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Mesh>& meshes, Intersection& closestIntersection) {
>>>>>>> Stashed changes
    bool hasIntersection = false;
    double closestDistance = std::numeric_limits<double>::max();

    for (const auto& sphere : spheres) {
        Intersection intersection(0, Vec3());
        if (sphere.intersect(ray, intersection) && intersection.distance < closestDistance) {
            closestDistance = intersection.distance;
            closestIntersection = intersection;
            hasIntersection = true;
        }
    }

    for (const auto& plane : planes) {
        Intersection intersection(0, Vec3());
        if (plane.intersect(ray, intersection) && intersection.distance < closestDistance) {
            closestDistance = intersection.distance;
            closestIntersection = intersection;
            hasIntersection = true;
        }
    }

    return hasIntersection;
}

void render(const Camera& camera, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, std::vector<unsigned char>& image) {
    std::cout << "Iniciando renderização..." << std::endl;

    for (int y = 0; y < camera.vres; ++y) {
        for (int x = 0; x < camera.hres; ++x) {
            Ray ray = camera.getRay(x, y);

            Intersection closestIntersection(0, Vec3());
            if (findClosestIntersection(ray, spheres, planes, closestIntersection)) {
                int index = 4 * (y * camera.hres + x);
                image[index + 0] = static_cast<unsigned char>(closestIntersection.color.x * 255);
                image[index + 1] = static_cast<unsigned char>(closestIntersection.color.y * 255);
                image[index + 2] = static_cast<unsigned char>(closestIntersection.color.z * 255);
                image[index + 3] = 255;
            } else {
                int index = 4 * (y * camera.hres + x);
                image[index + 0] = 0;
                image[index + 1] = 0;
                image[index + 2] = 0;
                image[index + 3] = 255;
            }
        }
    }

    std::cout << "Renderização concluída." << std::endl;
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
        Sphere(Point3(-1.5, -0.5, -2.5), 0.6, Vec3(0, 0, 0)),
        Sphere(Point3(0.8, -0.9, -3), 0.8, Vec3(0, 1, 0)),
        Sphere(Point3(0.5, -0.5, -1.5), 0.28, Vec3(1, 1, 0)),
        Sphere(Point3(1.5, 2, -3), 0.58, Vec3(0, 1, 1)),
        Sphere(Point3(2.5, 1, -5), 0.48, Vec3(1, 0, 0))
    };
    std::vector<Plane> planes = {
        Plane(Point3(0, -1, 0), Vec3(0, 1, 0), Vec3(1, 0, 1))
    };

    std::vector<unsigned char> image(camera.hres * camera.vres * 4);
<<<<<<< Updated upstream
    render(camera, spheres, planes, image);

    std::cout << "Salvando a imagem em formato PNG..." << std::endl;
    // Salva a imagem usando lodepng
    unsigned error = lodepng::encode("output.png", image, camera.hres, camera.vres);
    if (error) {
        std::cout << "Encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    } else {
        std::cout << "Imagem PNG salva com sucesso." << std::endl;
    }

    std::cout << "Salvando a imagem em formato PPM..." << std::endl;
    // Salva a imagem em formato PPM
    std::ofstream ppm("output.ppm");
    if (ppm.is_open()) {
        ppm << "P3\n" << camera.hres << " " << camera.vres << "\n255\n";
        for (int y = 0; y < camera.vres; ++y) {
            for (int x = 0; x < camera.hres; ++x) {
                int index = 4 * (y * camera.hres + x);
                int ir = static_cast<int>(image[index]);
                int ig = static_cast<int>(image[index + 1]);
                int ib = static_cast<int>(image[index + 2]);
                ppm << ir << ' ' << ig << ' ' << ib << '\n';
            }
=======
    
    // Renderiza a cena original
    render(camera, spheres, planes, meshes, image);
    lodepng::encode("output_before.png", image, camera.hres, camera.vres);
    std::ofstream ppm_before("output_before.ppm");
    ppm_before << "P3\n" << camera.hres << " " << camera.vres << "\n255\n";
    for (int y = 0; y < camera.vres; ++y) {
        for (int x = 0; x < camera.hres; ++x) {
            int index = 4 * (y * camera.hres + x);
            int ir = static_cast<int>(image[index]);
            int ig = static_cast<int>(image[index + 1]);
            int ib = static_cast<int>(image[index + 2]);
            ppm_before << ir << ' ' << ig << ' ' << ib << '\n';
>>>>>>> Stashed changes
        }
    }
    ppm_before.close();

    // Aplica uma transformação que combina translação, escala e rotação
    Matrix4 transform = Matrix4::translation(1, 1, 0) *
                        Matrix4::scaling(1.5, 1.5, 1.5) *
                        Matrix4::rotationY(M_PI / 4);  // Rotaciona 45 graus (PI/4 radianos) ao redor do eixo Y
    applyTransformation(transform, spheres, planes, meshes);

    // Renderiza a cena transformada
    render(camera, spheres, planes, meshes, image);
    lodepng::encode("output_after.png", image, camera.hres, camera.vres);
    std::ofstream ppm_after("output_after.ppm");
    ppm_after << "P3\n" << camera.hres << " " << camera.vres << "\n255\n";
    for (int y = 0; y < camera.vres; ++y) {
        for (int x = 0; x < camera.hres; ++x) {
            int index = 4 * (y * camera.hres + x);
            int ir = static_cast<int>(image[index]);
            int ig = static_cast<int>(image[index + 1]);
            int ib = static_cast<int>(image[index + 2]);
            ppm_after << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    ppm_after.close();

    return 0;
}