#include "BicubicBezierPatch.h"
#include "BicubicBezierIsoLine.h"
#include "../core/Materials.h"
#include "../Shader/ShaderPrograms.h"
#include "../core/GenericCurves3.h"

namespace cagd
{
    class BicubicBezierCompositeSurfaces3
    {
    public:
        class PatchAttributes{
        public:
            BicubicBezierPatch* patch;
            TriangulatedMesh3* mesh;
            Material *material;
            ShaderProgram *shader;
            RowMatrix<BicubicBezierIsoLine*> *u_isoLines;
            RowMatrix<BicubicBezierIsoLine*> *v_isoLines;
            RowMatrix<BicubicBezierPatch*> neighbors; // 8 elem
            PatchAttributes(): patch(0), mesh(0),material(0),shader(0),u_isoLines(0),v_isoLines(0),
                neighbors(RowMatrix<BicubicBezierPatch*>(8))
            {}
        };

    protected:
        std::vector<PatchAttributes> _patches;
        int patchCount;
    public:
        BicubicBezierCompositeSurfaces3();
        GLboolean insertPatch(BicubicBezierPatch bp);
        GLboolean continuePatch(int index);
        GLboolean joinPatches(int p1index, int p2index);
        GLboolean setPatch(int pindex,PatchAttributes pPatch);
    };

    BicubicBezierCompositeSurfaces3::BicubicBezierCompositeSurfaces3(){
        patchCount = 0;
    }

    GLboolean BicubicBezierCompositeSurfaces3::insertPatch(BicubicBezierPatch bp){
        (_patches[patchCount]).patch=&bp;
        patchCount++;
        _patches.resize(patchCount);

        return true;
    }

    GLboolean BicubicBezierCompositeSurfaces3::joinPatches(int i,int j){

        return true;
    }
}
