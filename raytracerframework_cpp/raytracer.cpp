//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//    
//  Students:
//    Guillaume Gosset
//    Quentin Lasota
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "torus.h"
#include "cone.h"
#include "csg.h"
#include "triangle.h"
#include "disc.h"
#include "plane.h"
#include "mesh.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>
#include <exception>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Camera* Raytracer::parseCamera(const YAML::Node& node) 
{
	Point eye = parseTriple(node["eye"]);
	Point center = parseTriple(node["center"]);
	Vector up = parseTriple(node["up"]);
	double xStretch;
	
	try {
		node["xStretch"] >> xStretch;
	}
	catch (std::exception e) {
		xStretch = 1;
	}

	int apertureSize;
	int apertureSample;
	try
	{
		node["AppertureSize"] >> apertureSize;
	}
	catch(std::exception e)
	{
		apertureSize = 0;
	}

	if(apertureSize == 0)
	{
		apertureSample = 1;
	}
	else
	{
		try
		{
			node["ApertureSample"] >> apertureSample;
		}
		catch(std::exception e)
		{
			apertureSample = 1;
		}
	}

	uint32_t viewWidth;
	uint32_t viewHeight;
	node["viewSize"][0] >> viewWidth;
	node["viewSize"][1] >> viewHeight;

	double exposureTime;
	try {
		node["exposureTime"] >> exposureTime;
	}
	catch (std::exception e) {
		exposureTime = 0.0;
	}

	Camera *cam = new Camera(eye, center, up, xStretch, viewWidth, viewHeight, exposureTime,apertureSize,apertureSample);
	return cam;
}

renderMode_t Raytracer::parseRenderMode(const YAML::Node& node)
{
	if (node == "phong")
		return phong;
	else if (node == "zbuffer")
		return zBuffer;
	else if (node == "zbufferAuto")
		return zBufferAuto;
	else if (node == "normal")
		return normal;
	else if (node == "uvBuffer")
		return uvBuffer;
	else if (node == "gooch")
		return gooch;
	else
		return phong;
}

bool Raytracer::parseShadows(const YAML::Node& node)
{
	if (node.GetType() != YAML::CT_SCALAR) return false;
	return node;
}

int Raytracer::parseMaxRecursionDepth(const YAML::Node& node)
{
	if (node.GetType() != YAML::CT_SCALAR) return 0;

	int i;
	node >> i;
	if (i >= 0 && i <= 20) return i;
	return 0;
}

float Raytracer::parseGoochParams(const YAML::Node& node)
{
	float toReturn;
	node >> toReturn;

	if (toReturn >= 0 && toReturn <= 1) return toReturn;
	return 0;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
	Material *m = new Material();

	try {
		node["color"] >> m->color;
	}
	catch (exception e) {
		m->color = Color(0, 0, 0);
	}
	node["ka"] >> m->ka;
	node["kd"] >> m->kd;
	node["ks"] >> m->ks;
	node["n"] >> m->n;

	try {
		node["refract"] >> m->refract;
	}
	catch (std::exception e) {
		m->refract = false;
	}
	if (m->refract) node["eta"] >> m->eta;

	try {
		node["alpha"] >> m->alpha;
	}
	catch (std::exception e) {
		m->alpha = 1.0;
	}

	try {
		string textureFile = node["texture"];
		m->texture = new Image(textureFile.c_str());
	}
	catch (std::exception e) {
		m->texture = nullptr;
	}

	try {
		string bumpFile = node["bump"];
		m->bump = new Image(bumpFile.c_str());
	}
	catch (std::exception e) {
		m->bump = nullptr;
	}

    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node,bool subobject = false)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

	Point pos;
	Triple rot;
	Triple vel;
	node["position"] >> pos;

	try { //optional rotation
		node["rotation"] >> rot;
	}
	catch (std::exception e) {
		rot = Triple(0.0, 0.0, 0.0);
	}

	try {
		node["velocity"] >> vel;
	}
	catch (std::exception e) {
		vel = Triple(0.0, 0.0, 0.0);
	}

	if (objectType == "sphere") {
		double r;

		const YAML::Node& rad = node["radius"];
		rad >> r;
		Sphere *sphere = new Sphere(pos, rot, vel, r);
		returnObject = sphere;
	}
	if (objectType == "disc") {
		double r;

		const YAML::Node& rad = node["radius"];
		rad >> r;
		Disc *disc = new Disc(pos, rot, vel, r);
		returnObject = disc;
	}
	if (objectType == "plane") 
	{
		long double height;
		long double width;
		try
		{
			node["height"] >> height ;
		}
		catch(std::exception e)
		{
			height = -1;
		}

		try
		{
			node["width"] >> width ;
		}
		catch(std::exception e)
		{
			width = -1;
		}
		
		Plane *plane = new Plane(pos, rot, vel,height,width);
		returnObject = plane;
	}
	if (objectType == "triangle") {
		Point v1,v2,v3;

		node["v1"] >> v1;
		node["v2"] >> v2;
		node["v3"] >> v3;
		Triangle *triangle = new Triangle(pos, rot, vel, v1,v2,v3);
		returnObject = triangle;
	}
	if (objectType == "torus") {
		double R;
		double r;
		node["wideRadius"] >> R;
		node["smallRadius"] >> r;
		Torus *torus = new Torus(pos, rot, vel, r, R);
		returnObject = torus;
	}
	if (objectType == "cone") {
		double r;
		double h;
		node["radius"] >> r;
		node["height"] >> h;
		Cone *cone = new Cone(pos, rot, vel, r, h);
		returnObject = cone;
	}
	if (objectType == "mesh") {
		string file = node["file"];
		char *cstr = new char[file.length() + 1];
		strcpy(cstr, file.c_str());
		float sc = node["scale"];
		Mesh *mesh = new Mesh(pos, rot, vel, cstr, sc);
		returnObject = mesh;
		delete[] cstr;
	}
	if(objectType == "csg")
	{
		Csg *csg = new Csg(pos,rot,vel);

		const YAML::Node& subObjects = node["Composants"];
        if (subObjects.GetType() != YAML::CT_SEQUENCE) {
            cerr << "Error: expected a sequence of objects." << endl;
            throw new std::exception;
        }
        for(YAML::Iterator it=subObjects.begin();it!=subObjects.end();++it)
		{
			Object* inerElement = parseObject((*it)["object"],true);
			if((*it)["mode"] == "union")
				csg->addElement(inerElement,Union);
			else if((*it)["mode"] == "intersect")
			{
				csg->intersectCount ++;
				csg->addElement(inerElement,Intersection);
			}
			else
				csg->addElement(inerElement,Difference);
		}
		returnObject = csg;
	}

    if (returnObject && !subobject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Triple size;
    try
    {
        node["size"] >> size;
    }
    catch(std::exception e)
    {
        size = Triple(0.0,0.0,0.0);
    }
    Color color;
    node["color"] >> color;
    return new Light(position,size,color);
}

int Raytracer::parseSSfactor(const YAML::Node& node)
{
    int factor;
    node["factor"] >> factor;
    return factor;
}
/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // Read scene configuration options
			try {
				const YAML::Node& cam = doc["Camera"];
				scene->setHasCamera(true);
				scene->setCamera(parseCamera(cam));
			}
			catch (std::exception e) {
				scene->setHasCamera(false);
				scene->setEye(parseTriple(doc["Eye"]));
			}

			try { 
				scene->setShadows(parseShadows(doc["Shadows"])); 
			}
			catch (std::exception e) { 
				scene->setShadows(false); 
			}

			try { 
				scene->setRenderMode(parseRenderMode(doc["RenderMode"]));
				if(scene->getRenderMode() == gooch)
				{
					scene->setB(parseGoochParams(doc["GoochParameters"]["b"]));
					scene->setY(parseGoochParams(doc["GoochParameters"]["y"]));
					scene->setAlpha(parseGoochParams(doc["GoochParameters"]["alpha"]));
					scene->setBeta(parseGoochParams(doc["GoochParameters"]["beta"]));
				}
			}
			catch (std::exception e) { 
				scene->setRenderMode(phong);
			}
			if (scene->getRenderMode() == zBuffer)
				scene->setClippingPlanes(doc["far"], doc["near"]);

			try { 
				scene->setMaxRecursionDepth(parseMaxRecursionDepth(doc["MaxRecursionDepth"]));
			}
			catch (std::exception e) { 
				scene->setMaxRecursionDepth(0);
			}

            try { 
				scene->setSuperSamplingFactor(parseSSfactor(doc["SuperSampling"]));
            }
            catch(std::exception e) { 
				scene->setSuperSamplingFactor(1);
            }

            try {
                scene->setLightSampling(doc["lightsSamples"]);
            }
            catch(std::exception e) {
                scene->setLightSampling(1);
            }

			try {
				scene->setExposureSamples(doc["exposureSamples"]);
			}
			catch (std::exception e) {
				scene->setExposureSamples(1);
			}

			try
			{
				scene->setEdgeLines(doc["edgeLines"]);
			}
			catch(std::exception e)
			{
				scene->setEdgeLines(false);
			}
			try
			{
				scene->setLensFlare(doc["LensFlare"]);
			}
			catch(std::exception e)
			{
				scene->setLensFlare(false);
			}

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneLights.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
	int w = 400;
	int h = 400;

	if (scene->getHasCamera()) {
		w = scene->getWidth();
		h = scene->getHeight();
	}
    Image img(w,h);
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl << endl;
}
