#ifndef GTRI_MODEL_IO_H
#define GTRI_MODEL_IO_H

#include <QString>
#include "core_global.h"

namespace Tomo
{

class GTriModel;

// ��GoCAD�ļ��м���������ģ��
bool CORE_EXPORT gLoadFromGoCAD(GTriModel *pModel, const QString &fname, QString &msg);

// ��OFF�ļ��м���������ģ��
bool CORE_EXPORT gLoadFromOFF(GTriModel *pModel, const QString &fname, QString &msg);


}

#endif
