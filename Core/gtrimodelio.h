#ifndef GTRI_MODEL_IO_H
#define GTRI_MODEL_IO_H

#include <QString>
#include "core_global.h"

namespace Tomo
{

class GTriModel;

// 从GoCAD文件中加载三角网模型
bool CORE_EXPORT gLoadFromGoCAD(GTriModel *pModel, const QString &fname, QString &msg);

// 从OFF文件中加载三角网模型
bool CORE_EXPORT gLoadFromOFF(GTriModel *pModel, const QString &fname, QString &msg);


}

#endif
