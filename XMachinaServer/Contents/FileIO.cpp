#include "pch.h"
#include "FileIO.h"

#include "ResourceManager.h"

namespace {
    sptr<Model> LoadFrameHierarchy(std::ifstream& file)
    {
        sptr<Model> model = std::make_shared<Model>();

        std::string token;
        FileIO::ReadString(file, token); // <Frame>:
        {
            FileIO::ReadString(file, token); // <Name>:
            FileIO::ReadString(file, model->mName);

            FileIO::ReadString(file, token); // <Matrix>:
            FileIO::ReadVal(file, model->mTransform);

            FileIO::ReadString(file, token); // <BoundingSpheres>:
            int sphereCnt = FileIO::ReadVal<int>(file);

            if(sphereCnt > 0)
            {
                std::vector<Vec3> centers;
                std::vector<float> radiuses;
                FileIO::ReadRange(file, centers, sphereCnt);
                FileIO::ReadRange(file, radiuses, sphereCnt);

                MyBoundingSphere bs{};
                for (int i = 0; i < sphereCnt; ++i) {
                    bs.SetOrigin(centers[i]);
                    bs.Center = centers[i];
                    bs.Radius = radiuses[i];

                    model->mBSList.push_back(bs);
                }
            }

            FileIO::ReadString(file, token); // <BoundingBoxes>:
            int boxCnt = FileIO::ReadVal<int>(file);

            if (boxCnt > 0)
            {
                std::vector<Vec3> centers;
                std::vector<Vec3> extents;
                FileIO::ReadRange(file, centers, boxCnt);
                FileIO::ReadRange(file, extents, boxCnt);

                MyBoundingOrientedBox box{};
                for (int i = 0; i < boxCnt; ++i) {
                    box.SetOrigin(centers[i]);
                    box.Center = centers[i];
                    box.Extents = extents[i];

                    model->mBoxList.push_back(box); 
                }
            }
        }


        FileIO::ReadString(file, token); // <Child>:
        int childCnt = FileIO::ReadVal<int>(file);

        for (int i = 0; i < childCnt; ++i) {
            sptr<Model> child = LoadFrameHierarchy(file);
            model->SetChild(child);
        }

        FileIO::ReadString(file, token); // </Frame>:

        return model;
    }
}


sptr<Model> FileIO::LoadGeometryFromFile(const std::string& filePath)
{
    std::ifstream file = OpenBinFile(filePath);

    sptr<Model> model = std::make_shared<Model>();

    std::string token;

    FileIO::ReadString(file, token); // <Hierarchy>:

    model = ::LoadFrameHierarchy(file);

    FileIO::ReadString(file, token); // <\Hierarchy>:

    return model;
}
