#include <QtCore/QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QStringList>
#include "gtrimodelio.h"
#include "gtriLTI.h"
#include "gtrimodel.h"

void help()
{
	printf("How to use it:\n");
	printf("The program need 3 parameters as input\n");
	printf("argv[1]: the model file, whose extension is .ts(GoCAD export Surface) or .off;\n");
	printf("argv[2]: the coordinate file, which stores the coordinates of the source point in the first line\n");
	printf("and that of receiver points in the following lines;\n");
	printf("argv[3]: the output directory path, if not exists, the program create one automatically.\n");
	printf("When finish running, the program generates several files which stores the ray paths corresponding to each pair of source point and receiver point.\n");
}

int main(int argc, char *argv[])
{
	printf("High-accuracy ray tracing method using quadratic traveltime interpolation on triangular mesh\n");
	printf("*****************************************************************************\n");
	printf("Copyright (c) 2020, Deng Fei, Fang Peng. All rights reserved.\n");
	printf("QTI is available under two different licenses:\n");
	printf("* GNU General Public License v3.0 (GPLv3)\n");
	printf("* Commercial license (please contact at Deng Fei(at)dengfei@cdut.cn)\n");
	printf("*****************************************************************************\n");
	if (argc < 4)
	{
		help();
		return -1;
	}

	QCoreApplication a(argc, argv);
	QString filename = argv[1];
	QString msg = "WARNING";
	Tomo::GTriModel *mpModel = new Tomo::GTriModel();
	QFileInfo fi(filename);

	// load model file
	if (fi.suffix().toLower() == "ts")
	{
		if (!gLoadFromGoCAD(mpModel, filename, msg))
		{
			qWarning("Load \"%s\" Failed!", filename.toLocal8Bit().data());
		}
	}
	else if (fi.suffix().toLower() == "off")
	{
		gLoadFromOFF(mpModel, filename, msg);
	}
	else
	{
		qWarning("File type Error!");
		return a.exec();
	}

	QVector<GPoint3d> spts(1);
	QVector<GPoint3d> rpts;
	QString paramFile = argv[2];
	QFile file(paramFile);
	if(!file.open(QFile::ReadOnly)) 
	{
		qWarning("Can not open parameter file!");
		return a.exec();
	}
	else
	{
		//read the coordinates of source point and receiver points from parameter file
		QTextStream stream(&file);
		QString line;
		stream >> line;
		if (line.trimmed().isEmpty())
		{
			qWarning("parameter file format error!");
			return a.exec();
		}
		QStringList pt0list = line.split(",");
		double x0 = pt0list[0].toDouble();
		double z0 = pt0list[1].toDouble();
		double y0 = pt0list[2].toDouble();
		spts[0].setXYZ(x0, y0, z0);
		while (!stream.atEnd())
		{
			stream >> line;
			if (line.trimmed().isEmpty()) continue ;
			QStringList ptlist = line.split(",");
			double x = ptlist[0].toDouble();
			double z = ptlist[1].toDouble();
			double y = ptlist[2].toDouble();
			GPoint3d cpt(x, y, z);
			rpts << cpt;
		}
	}
	mpModel->putSurveyToTri(spts);
	mpModel->putSurveyToTri(rpts);
	for (int i=rpts.count()-1; i>=0; i--)
	{
		if (rpts[i].getUsrInt() < 0) rpts.remove(i);
	}

	QVector<double> fbts;
	QList<QVector<GPoint3d> > rays;
	
	Tomo::GTriLTI triLTI(mpModel);
	triLTI.setModel(mpModel);
	mpModel->fillSlowness(0.001);

	//perform QTI method
	triLTI.forwardQTI(spts[0], rpts, fbts, rays);
	const QString outFilepath = argv[3];
	QDir dir;
	if(!dir.exists(outFilepath))
	{
		if(!dir.mkdir(outFilepath))
		{
			qWarning("output file path error!");
			return a.exec();;
		}
	}
	// output ray paths
	for (int i = 0; i < rays.count(); i++)
	{
		QString fname3 = outFilepath + "/rays" + QString::number(i) + ".txt";
		QFile file3(fname3);
		if (!file3.open(QFile::WriteOnly)) continue;
		QTextStream stream(&file3);
		QVector<GPoint3d> &ray = rays[i];
		printf("ray[%d] count: %d\n",i, ray.count());
		for (int j = 0; j < ray.count(); j++)
		{
			stream << ray[j].x() << " " << ray[j].y() << " " << ray[j].z() << endl;
		}
	}
	return a.exec();
}
