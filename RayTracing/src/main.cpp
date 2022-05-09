#include <iostream>
#include <limits>

#include "common/vec3.h"
#include "common/triangle.h"
#include "common/objreader.h"
#include "common/camera.h"
#include "common/object.h"
#include "common/objectlist.h"
#include "common/sphere.h"
#include "common/simpletriangle.h"
#include "common/image.h"
#include "common/raytracer.h"
#include "common/plane.h"

int main (const int argc, const char *argv[]) {

    // Read file
    ObjReader obj("../models/cornellBox.obj");
    if (!obj.IsSuccess()) { std::cerr << "failed" << std::endl; return -1; }

    // std::vector<Triangle> tri = obj.GetTris();
    std::vector<std::shared_ptr<Triangle>> tri = obj.GetTris();

    ObjectList world;
    // world.add(std::make_shared<Sphere>(Point3(-1,0,0), 0.5)); // left sphere
    // world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5)); // middle sphere
    // world.add(std::make_shared<Sphere>(Point3(1,0,0), 0.5)); // right sphere
    // world.add(std::make_shared<Sphere>(Point3(0,-100.5,0), 100)); // ground

    // Back wall
    world.add(std::make_shared<Plane>(Point3(-2.0f, -1.0f, 2.0f), Point3(-2.0f, 1.0f, 2.0f), Point3(2.0f, -1.0f, 2.0f), Point3(2.0f, 1.0f, 2.0f), Dir3(0.0, 0.0, -1.0)));

    // Left wall
    world.add(std::make_shared<Plane>(Point3(-2.0f, -1.0f, -5.0f), Point3(-2.0f, 1.0f, -5.0f), Point3(-2.0f, -1.0f, 2.0f), Point3(-2.0f, 1.0f, 2.0f), Dir3(1.0, 0.0, 0.0)));

    // Right wall
    world.add(std::make_shared<Plane>(Point3(2.0f, -1.0f, -5.0f), Point3(2.0f, 1.0f, -5.0f), Point3(2.0f, -1.0f, 2.0f), Point3(2.0f, 1.0f, 2.0f), Dir3(-1.0, 0.0, 0.0)));

    // Bottom wall
    world.add(std::make_shared<Plane>(Point3(-2.0f, -1.0f, -5.0f), Point3(-2.0f, -1.0f, 2.0f), Point3(2.0f, -1.0f, -5.0f), Point3(2.0f, -1.0f, 2.0f), Dir3(0.0, 1.0, 0.0)));

    // Top wall
    world.add(std::make_shared<Plane>(Point3(-2.0f, 1.0f, -5.0f), Point3(-2.0f, 1.0f, 2.0f), Point3(2.0f, 1.0f, -5.0f), Point3(2.0f, 1.0f, 2.0f), Dir3(0.0, -1.0, 0.0)));

    // Sphere
    world.add(std::make_shared<Sphere>(Point3(-1,-0.5,0), 0.5)); // sphere
    world.add(std::make_shared<Sphere>(Point3(1,-0.5,0), 0.5)); // sphere

    // Materials
    // Left wall
    world.objects[1]->Ambient = Colour3(0.5f, 0.0f, 0.0f);
    world.objects[1]->Diffuse = Colour3(1.0f, 0.0f, 0.0f);

    // Right wall
    world.objects[2]->Ambient = Colour3(0.0f, 0.5f, 0.0f);
    world.objects[2]->Diffuse = Colour3(0.0f, 1.0f, 0.0f);

    // Bottom wall
    world.objects[3]->Impulse = 1.1f;
    world.objects[3]->ImpulseAlbedo = 1.0f;

    // Left Sphere
    world.objects[5]->Impulse = 1.1f;
    world.objects[5]->ImpulseAlbedo = 1.0f;

    // // Left sphere
    // world.objects[0]->Ambient = Colour3(0.5f, 0.0f, 0.0f);
    // world.objects[0]->Diffuse = Colour3(1.0f, 0.0f, 0.0f);
    // world.objects[0]->Impulse = 1.1f;
    // world.objects[0]->ImpulseAlbedo = 1.0f;

    // // Middle sphere
    // world.objects[1]->Ambient = Colour3(0.0f, 0.5f, 0.0f);
    // world.objects[1]->Diffuse = Colour3(0.0f, 1.0f, 0.0f);

    // // Right sphere
    // world.objects[2]->Ambient = Colour3(0.0f, 0.0f, 0.5f);
    // world.objects[2]->Diffuse = Colour3(0.0f, 0.0f, 1.0f);

    // // Background
    // world.objects[4]->Ambient = Colour3(0.5f, 0.0f, 0.0f);
    // world.objects[4]->Diffuse = Colour3(1.0f, 0.0f, 0.0f);

    // Render
    // Image
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;

    Image img(aspect_ratio, image_width, image_height, samples_per_pixel);
    Camera cam; /// Default camera

    RayTracer rayTracer(world, cam, img);
    return rayTracer.Exec();
}