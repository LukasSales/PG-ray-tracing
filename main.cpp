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
#include "mesh.h"
#include "matrix4.h"
#include "transformation.h"
#include "material.h"
#include "light.h"
#include "utils.h"
#include "torus.h"
#include <fstream>

#define M_PI 3.14159265358979323846

// Função para calcular a iluminação de Phong
Vec3 calculatePhongIllumination(const Point3& point, const Vec3& normal, const Material& material, const Vec3& viewDir, const std::vector<Light>& lights, const Vec3& ambientLight) {
    Vec3 ambient = material.ka * ambientLight;
    Vec3 diffuse(0, 0, 0);
    Vec3 specular(0, 0, 0);

    for (const auto& light : lights) {
        Vec3 lightDir = (light.position - point).normalize();
        double diff = std::max(normal.dot(lightDir), 0.0);
        diffuse += light.intensity * material.kd * diff;

        Vec3 reflectDir = (2 * normal.dot(lightDir) * normal - lightDir).normalize();
        double spec = std::pow(std::max(viewDir.dot(reflectDir), 0.0), material.eta);
        specular += light.intensity * material.ks * spec;
    }

    return ambient + diffuse + specular;
}

// Função para encontrar a interseção mais próxima entre um raio e uma lista de objetos
bool findClosestIntersection(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Mesh>& meshes, const std::vector<Torus>& tori, Intersection& closestIntersection) {
    bool hasIntersection = false;
    double closestDistance = std::numeric_limits<double>::max();

    // Verifica interseção com esferas
    for (const auto& sphere : spheres) {
        Intersection intersection(0, Vec3(), Material());
        if (sphere.intersect(ray, intersection) && intersection.distance < closestDistance) {
            closestDistance = intersection.distance;
            closestIntersection = intersection;
            hasIntersection = true;
        }
    }

    // Verifica interseção com planos
    for (const auto& plane : planes) {
        Intersection intersection(0, Vec3(), Material());
        if (plane.intersect(ray, intersection) && intersection.distance < closestDistance) {
            closestDistance = intersection.distance;
            closestIntersection = intersection;
            hasIntersection = true;
        }
    }

    // Verifica interseção com malhas
    for (const auto& mesh : meshes) {
        Intersection intersection;
        if (mesh.intersect(ray, intersection) && intersection.distance < closestDistance) {
            closestDistance = intersection.distance;
            closestIntersection = intersection;
            hasIntersection = true;
        }
    }

    // Verifica interseção com toros
    for (const auto& torus : tori) {
        Intersection intersection;
        if (torus.intersect(ray, intersection) && intersection.distance < closestDistance) {
            closestDistance = intersection.distance;
            closestIntersection = intersection;
            hasIntersection = true;
        }
    }

    return hasIntersection;
}

// Função de traçado de raios
Vec3 traceRay(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Mesh>& meshes, const std::vector<Torus>& tori, const std::vector<Light>& lights, const Vec3& ambientLight, int depth, int maxDepth) {
    if (depth > maxDepth) return Vec3(1, 1, 1); // Limita a profundidade de recursão

    Intersection closestIntersection; 
    if (findClosestIntersection(ray, spheres, planes, meshes, tori, closestIntersection)) {
        Point3 point = ray.at(closestIntersection.distance);
        Vec3 normal = closestIntersection.normal;
        Vec3 viewDir = -ray.direction;
        Vec3 color = calculatePhongIllumination(point, normal, closestIntersection.material, viewDir, lights, ambientLight);

        // Reflexão
        if (closestIntersection.material.kr.dot(Vec3(1, 1, 1)) > 0) {
            Vec3 reflectDir = reflect(ray.direction, normal);
            Ray reflectRay(point + reflectDir * 1e-4, reflectDir);
            color += closestIntersection.material.kr * traceRay(reflectRay, spheres, planes, meshes, tori, lights, ambientLight, depth + 1, maxDepth);
        }

        // Refração
        if (closestIntersection.material.refraction > 0) {
            double ior = closestIntersection.material.refraction; // Índice de refração
            Vec3 refractDir = refract(ray.direction, normal, ior);
            Ray refractRay(point + refractDir * 1e-4, refractDir);
            color += closestIntersection.material.kt * traceRay(refractRay, spheres, planes, meshes, tori, lights, ambientLight, depth + 1, maxDepth);
        }

        return color;
    }

    return Vec3(0, 0, 0); // Cor de fundo
}

// Função de renderização
void render(const Camera& camera, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Mesh>& meshes, const std::vector<Torus>& tori, const std::vector<Light>& lights, const Vec3& ambientLight, std::vector<unsigned char>& image, int maxDepth) {
    int width = camera.hres;
    int height = camera.vres;
    Vec3 backgroundColor(0, 0, 0); // Define a cor de fundo como branco

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            Vec3 color = traceRay(ray, spheres, planes, meshes, tori, lights, ambientLight, 0, maxDepth);

            // Preenchendo a imagem
            image[(j * width + i) * 4 + 0] = static_cast<unsigned char>(std::min(color.x, 1.0) * 255);
            image[(j * width + i) * 4 + 1] = static_cast<unsigned char>(std::min(color.y, 1.0) * 255);
            image[(j * width + i) * 4 + 2] = static_cast<unsigned char>(std::min(color.z, 1.0) * 255);
            image[(j * width + i) * 4 + 3] = 255;
        }
    }
}

int main() {
    // Definições da câmera
    Point3 cameraPosition(0, 0, 0);
    //Point3 cameraPosition(0, 2, 5); //camera distante
    Point3 lookAt(0, 0, -1);
    Vec3 up(0, 1, 0);
    double distance = 1.0;
    int vres = 500;
    int hres = 500;

    Camera camera(cameraPosition, lookAt, up, distance, vres, hres);

    // Definir as luzes
    std::vector<Light> lights = {
        Light(Point3(5, 5, -5), Vec3(1, 1, 1))
    };

    // Definir a luz ambiente
    Vec3 ambientLight(0.1, 0.1, 0.1);

    // Definir os materiais
    Material red_diffuse(Vec3(1.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), Vec3(0.1, 0.1, 0.1), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), 10.0, 1.0);
    Material white(Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.5, 0.5), Vec3(0.1, 0.1, 0.1), Vec3(0.5, 0.5, 0.5), Vec3(0.0, 0.0, 0.0), 32, 0);
    Material green_specular(Vec3(0.0, 1.0, 0.0), Vec3(1.0, 1.0, 1.0), Vec3(0.1, 0.1, 0.1), Vec3(0.5, 0.5, 0.5), Vec3(0.0, 0.0, 0.0), 50.0, 1.0);
    Material blue_matte(Vec3(0.0, 0.0, 1.0), Vec3(0.1, 0.1, 0.1), Vec3(0.1, 0.1, 0.1), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), 10.0, 1.0);
    Material shiny_metal(Vec3(0.8, 0.8, 0.8), Vec3(1.0, 1.0, 1.0), Vec3(0.2, 0.2, 0.2), Vec3(1.0, 1.0, 1.0), Vec3(0.0, 0.0, 0.0), 100.0, 1.0);
    Material dull_plastic(Vec3(0.5, 0.5, 0.5), Vec3(0.0, 0.0, 0.0), Vec3(0.3, 0.3, 0.3), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), 5.0, 1.0);
    Material shiny_red(Vec3(1.0, 0.0, 0.0), Vec3(1.0, 1.0, 1.0), Vec3(0.1, 0.1, 0.1), Vec3(0.8, 0.8, 0.8), Vec3(0.0, 0.0, 0.0), 80.0, 1.0);
    Material glass(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), Vec3(0.1, 0.1, 0.1), Vec3(0.9, 0.9, 0.9), 100.0, 1.5);
    Material red(Vec3(1, 0, 0), Vec3(0.5, 0.5, 0.5), Vec3(0.1, 0.1, 0.1), Vec3(0.2, 0.2, 0.2), Vec3(0, 0, 0), 32, 0);
    Material green(Vec3(0, 1, 0), Vec3(0.5, 0.5, 0.5), Vec3(0.1, 0.1, 0.1), Vec3(0.2, 0.2, 0.2), Vec3(0, 0, 0), 32, 0);
    Material blue(Vec3(0, 0, 1), Vec3(0.5, 0.5, 0.5), Vec3(0.1, 0.1, 0.1), Vec3(0.2, 0.2, 0.2), Vec3(0, 0, 0), 32, 0);
    Material reflective(Vec3(1, 1, 1), Vec3(0.5, 0.5, 0.5), Vec3(0.1, 0.1, 0.1), Vec3(1, 1, 1), Vec3(0, 0, 0), 128, 0);
    Material refractive(Vec3(1, 1, 1), Vec3(0.5, 0.5, 0.5), Vec3(0.1, 0.1, 0.1), Vec3(0, 0, 0), Vec3(1, 1, 1), 32, 1.5);
    Material orange_translucent(Vec3(1.0, 0.5, 0.0), Vec3(0.5, 0.5, 0.5), Vec3(0.1, 0.1, 0.1), Vec3(0.1, 0.1, 0.1), Vec3(0.8, 0.8, 0.8), 30.0, 1.3);
    Material magenta_glossy(Vec3(1.0, 0.0, 1.0), Vec3(0.9, 0.9, 0.9), Vec3(0.3, 0.3, 0.3), Vec3(0.5, 0.5, 0.5), Vec3(0.0, 0.0, 0.0), 80.0, 1.0);
    Material black_metallic(Vec3(0.1, 0.1, 0.1), Vec3(0.8, 0.8, 0.8), Vec3(0.2, 0.2, 0.2), Vec3(1.0, 1.0, 1.0), Vec3(0.0, 0.0, 0.0), 100.0, 1.0);
    Material cyan_glass(Vec3(0.0, 1.0, 1.0), Vec3(0.1, 0.1, 0.1), Vec3(0.1, 0.1, 0.1), Vec3(0.0, 0.0, 0.0), Vec3(0.9, 0.9, 0.9), 64.0, 1.5);

    // Definir os objetos
     std::vector<Sphere> spheres = {
        /*Sphere(Point3(-1.5, -1.5, -1.85), 0.8, cyan_glass),
        Sphere(Point3(2.5, 2, -3.5), 0.8, blue_matte),
        Sphere(Point3(0, 2, -5), 1.0, red_diffuse),
        Sphere(Point3(2.5, 2.0, -5.0), 1.0, magenta_glossy) */
    };

    std::vector<Plane> planes = {
        Plane(Point3(0, -1, 0), Vec3(0, 1, 0), white)
    };

   std::vector<Torus> tori = {
    Torus(Point3(0, 0.0, 0), 0.3, 0.1, 10, 10, shiny_red),   // Toro menor
    //Torus(Point3(0.0, 0.0, -3.0), 0.5, 0.2, 20, 20, green_specular),    // Toro médio
    //Torus(Point3(2, 0.0, -3.0), 0.6, 0.3, 25, 25, cyan_glass)   // Toro maior
    };

    // Definições dos vértices e triplas da malha
    std::vector<Point3> vertices = {
       //Point3(-2, -1, -6), Point3(2, -1, -6), Point3(0, 2, -6) // Triângulo 1 no fundo

};
    std::vector<std::vector<int>> triplas = {
        //{0, 1, 2}, // Triângulo 1
    };

    std::vector<Mesh> meshes = {
        //Mesh(vertices, triplas, blue)
    };

    // Cria o buffer de imagem
    std::vector<unsigned char> image(camera.hres * camera.vres * 4);

    // Renderizar a cena
    int maxDepth = 5;
    render(camera, spheres, planes, meshes, tori, lights, ambientLight, image, 5);
    lodepng::encode("output_antes.png", image, camera.hres, camera.vres);
    std::ofstream ppm_before("output_antes.ppm");
    ppm_before << "P3\n" << camera.hres << " " << camera.vres << "\n255\n";
    for (int y = 0; y < camera.vres; ++y) {
        for (int x = 0; x < camera.hres; ++x) {
            int index = 4 * (y * camera.hres + x);
            int ir = static_cast<int>(image[index]);
            int ig = static_cast<int>(image[index + 1]);
            int ib = static_cast<int>(image[index + 2]);
            ppm_before << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    ppm_before.close();

    // Aplica uma transformação que combina translação, escala e rotação
    Matrix4 transform = Matrix4::translation(1, 1, 0) *
                        Matrix4::scaling(1.5, 1.5, 1.5) *
                        Matrix4::rotationZ(M_PI / 4);  // Rotaciona 45 graus (PI/4 radianos) ao redor do eixo Y
    applyTransformation(transform, spheres, planes, meshes, tori);

    // Renderiza a cena transformada
    render(camera, spheres, planes, meshes, tori, lights, ambientLight, image, maxDepth);
    lodepng::encode("output_depois.png", image, camera.hres, camera.vres);
    std::ofstream ppm_after("output_depois.ppm");
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