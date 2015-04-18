#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
//#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyle.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>

#include <QString>
#include <QFileDialog>
//#include <vtkStringArray.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    readerDCMSeries = vtkSmartPointer<vtkGDCMImageReader>::New();
    imageViewerDCMSeriesX = vtkSmartPointer<vtkImageViewer2>::New();
    imageShiftScale= vtkSmartPointer<vtkImageShiftScale>::New();
    arrayS =vtkSmartPointer<vtkStringArray>::New();
    convertITKLuminanceFilter =vtkSmartPointer<vtkImageLuminance>::New();
    readImageData=vtkSmartPointer<vtkImageData>::New();
    
    //TagDialog *externalTagDialog= new TagDialog;
    //connect(SedSignalToQDialog(),SIGNAL(SentSignal()),externalTagDialog,SLOT(RecSignalFromMainWindow()));
    SedSignalToQDialog();
    //viewTagWindow.setModal(false);
    //viewTagWindow.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::openDCMFolder()
{
    QDir dir ("Dir");
    dir=QFileDialog::getExistingDirectory(this,tr("Open DCM Folder"),QDir::currentPath(),QFileDialog::ShowDirsOnly);
    QFileInfoList list=dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    std::vector<std::string> names;
    foreach(QFileInfo finfo,list)
    {
	std::string str=dir.path().toUtf8().constData();
	str=str+"/";
	names.push_back(str+finfo.fileName().toUtf8().constData());
    }
    drawDCM(names);
}

void MainWindow::drawDCM(std::vector<std::string> &namess)
{
    for(unsigned int a=0;a<namess.size();a++)
    {
	std::cout<<namess[a]<<std::endl;
	arrayS->InsertNextValue(namess[a]);
    }
    //QString folderNameDCM = QFileDialog::getExistingDirectory(this,tr("Open DCM Folder"),QDir::currentPath(),QFileDialog::ShowDirsOnly);

    //std::string stdstrFolderNameDCM = folderNameDCM.toUtf8().constData();
    /*drawDCMSeries(stdstrFolderNameDCM);*/
    //array.InsertNextValeu()
    readerDCMSeries->SetFileNames(arrayS);
    readerDCMSeries->Update();

    //check if slice is RGB
    std::cout<<"Check number of Scalar "<<readImageData->GetNumberOfScalarComponents()<<std::endl;

    if(readImageData->GetNumberOfScalarComponents()==1)
    {
	std::cout<<"ScalarComponenets == 1"<<std::endl;
	imageViewerDCMSeriesX->SetInputConnection(readerDCMSeries->GetOutputPort());
    }
    else
    {
	LuminanceFilter();
	    }
    mMinSliderX = imageViewerDCMSeriesX->GetSliceMin();
    mMaxSliderX = imageViewerDCMSeriesX->GetSliceMax();
    ui->hSliderDCM->setMinimum(mMinSliderX);
    ui->hSliderDCM->setMaximum(mMaxSliderX);
    convertToITK();

    imageViewerDCMSeriesX->SetRenderWindow(ui->vtkRenderer->GetRenderWindow());
    imageViewerDCMSeriesX->Render();

    

}

void MainWindow::LuminanceFilter()
{
	std::cout<<"Inside LuminanceFilter"<<std::endl;
	convertITKLuminanceFilter->SetInputConnection(readerDCMSeries->GetOutputPort());
	convertITKLuminanceFilter->Update();
	imageViewerDCMSeriesX->SetInputConnection(convertITKLuminanceFilter->GetOutputPort());

}
void MainWindow::convertToITK()
{
    std::cout<<"Start convert to ITK"<<std::endl;
    VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
    vtkImageToImageFilter->SetInput(convertITKLuminanceFilter->GetOutput());
    vtkImageToImageFilter->Update();
    std::cout<<"End convert to ITK"<<std::endl;
}
void MainWindow::readPatientInformation(int position)

{
    std::cout<<"Read Patient Info"<<std::endl;
    itk::GDCMImageIO::Pointer dicomIO= itk::GDCMImageIO::New();
    ReaderType::Pointer readerInfoPatient=ReaderType::New();
    std::cout<<arrayS->GetValue(position)<<std::endl;
    readerInfoPatient->SetFileName(arrayS->GetValue(position));
    readerInfoPatient->SetImageIO(dicomIO);
    readerInfoPatient->Update();

    std::string tagvalue;
    dicomIO->GetValueFromTag("0010|0010",tagvalue);
    std::string patientName=tagvalue;
    std::string patientID =FindDicomTag("0010|0020",dicomIO);
    std::string patientSex =FindDicomTag("0010|0040",dicomIO);
    std::string patientAge =FindDicomTag("0010|1010",dicomIO);
    std::string patientDate =FindDicomTag("0008|0020",dicomIO);
    std::string studyDate =FindDicomTag("0008|0020",dicomIO);
    std::string modality = FindDicomTag("0009|0060",dicomIO);

    std::cout<<"Patient Name "<<patientName<<std::endl;
    std::cout<<"Patient ID "<<patientID<<std::endl;
    std::cout<<"Patient Sex "<<patientSex<<std::endl;
}

void MainWindow::on_btnOpenDCMFolder_clicked()
{
    openDCMFolder();
}

void MainWindow::on_hSliderDCM_sliderMoved(int position)
{
    std::cout<<"Slice number "<<imageViewerDCMSeriesX->GetSlice()<<std::endl;
    imageViewerDCMSeriesX->SetSlice(position);
    readPatientInformation(position);
    imageViewerDCMSeriesX->Render();
}

std::string MainWindow::FindDicomTag(const std::string & entryId, const itk::GDCMImageIO::Pointer dicomIO)
{

	std::string tagvalue;
	std::cout<<entryId<<std::endl;
	bool found = dicomIO->GetValueFromTag(entryId, tagvalue);
	std::cout<<dicomIO->GetValueFromTag(entryId, tagvalue)<<std::endl;
	if(!found)
	{
	    tagvalue="NOT FOUND";
	}
	return tagvalue;
}

void MainWindow::SedSignalToQDialog()
{
    qDebug()<<"Inside SedSignalToQDialog";
    TagDialog *externalTagDialog= new TagDialog(this);
//    connect(SedSignalToQDialog(),SIGNAL(SentSignal),externalTagDialog,SLOT(RecSignalFromMainWindow()));
    externalTagDialog->setModal(false);
    externalTagDialog->show();
}