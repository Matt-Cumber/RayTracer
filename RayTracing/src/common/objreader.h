#ifndef OBJREADER_H
#define OBJREADER_H

#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

#include "triangle.h"
#include "vec3.h"

// Constants to define characters in file
namespace {
const std::string VERTEX = "v";
const std::string NORMAL = "vn";
const std::string TEX = "vt";
const std::string FACE = "f";
const char FACE_SEPERATOR = '/';
}

/*
 * ObjReader
 * Simple obj reader. Just call constructor with file name. Then use get methods for data if valid
 */
class ObjReader {
public:
    ObjReader(const std::string &f) : m_success(true) {
        m_tri.clear();
        m_v.clear();
        m_n.clear();
        m_tc.clear();
        m_inf.open(f, std::ifstream::in);
        if (!m_inf.is_open()) return;
        std::string tok;
        while (m_inf >> tok >> std::ws) {
            if (tok == VERTEX) if (!ParseVertex()) { m_success = false; break; }
            if (tok == NORMAL) if (!ParseNormal()) { m_success = false; break; }
            if (tok == TEX) if (!ParseTex()) { m_success = false; break; }
            if (tok == FACE) if (!ParseFace()) { m_success = false; break; }
        }

        m_inf.close();
    }

    std::vector<std::shared_ptr<Triangle>> GetTris() const { return m_tri; };

    bool IsSuccess() const { return m_success; };

private:
    bool ParseValue(float &t) {
        float v;
        if (!(m_inf >> v)) return false;
        t = v;
        return true;
    }

    bool ParseValue2(float (&t)[2]) {
        if(!ParseValue(t[0])) return false;
        if(!ParseValue(t[1])) return false;
        return true;
    }

    bool ParseValue3(float (&t)[3]) {
        if(!ParseValue(t[0])) return false;
        if(!ParseValue(t[1])) return false;
        if(!ParseValue(t[2])) return false;
        return true;
    }

    bool ParseVertex() {
        float t[3];
        if(!ParseValue3(t)) return false;
        m_v.emplace_back(Point3(t[0], t[1], t[2]));
        return true;
    }

    bool ParseNormal() {
        float t[3];
        if(!ParseValue3(t)) return false;
        m_n.emplace_back(Dir3(t[0], t[1], t[2]));
        return true;
    }

    bool ParseTex() {
        float t[2];
        if(!ParseValue2(t)) return false;
        m_tc.emplace_back(Tex2(t[0], t[1], 0.0f));
        return true;
    }

    /*
     * face triple is v/vt/vn
     */
    bool ParseFaceTriple(std::string tok, int &u, int &v, int &w) {
        std::string t;
        std::stringstream ss(tok);
        if (!std::getline(ss, t, FACE_SEPERATOR)) return false;
        u = std::stoi(t);
        if (!std::getline(ss, t, FACE_SEPERATOR)) return false;
        v = std::stoi(t);
        if (!std::getline(ss, t, FACE_SEPERATOR)) return false;
        w = std::stoi(t);
        return true;
    }

    /*
     * indices start at 1 in file, but 0 in code, so subtract 1
     */
    void MakeTriangle(int v[3], int vt[3], int vn[3]) {
        std::array<Point3, 3> triv {m_v[v[0]-1], m_v[v[1]-1], m_v[v[2]-1]};
        std::array<Dir3, 3> trin {m_n[vn[0]-1], m_n[vn[1]-1], m_n[vn[2]-1]};
        std::array<Tex2, 3> tritc {m_tc[vt[0]-1], m_tc[vt[1]-1], m_tc[vt[2]-1]};
        m_tri.emplace_back(std::make_shared<Triangle>(triv, trin, tritc));
    }

    /*
     * Face is v/vt/vn v/vt/vn v/vt/vn
     */
    bool ParseFace() {
        int v[3];
        int vn[3];
        int vt[3];
        std::string tok;
        for(int i = 0; i < 3; ++i) {
            if(!(m_inf >> tok)) return false;
            if(!ParseFaceTriple(tok, v[i], vt[i], vn[i])) return false;
        }
        MakeTriangle(v, vt, vn);
        return true;
    }

private:
    std::ifstream m_inf;
    bool m_success;
    std::vector<std::shared_ptr<Triangle>> m_tri;
    std::vector<Point3> m_v;
    std::vector<Dir3> m_n;
    std::vector<Tex2> m_tc;
};

#endif