#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkGDCMImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkStringArray.h>
#include <itkVTKImageToImageFilter.h>
#include <vtkImageLuminance.h>
#include <vtkImageData.h>

#include <itkVTKImageToImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkImageFileReader.h>

#include "TagDialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openDCMFolder();
    void LuminanceFilter();
    void drawDCM(std::vector<std::string> &namess);
    void convertToITK();
    void on_btnOpenDCMFolder_clicked();
    void readPatientInformation(int position);
    void on_hSliderDCM_sliderMoved(int position);
    std::string FindDicomTag(const std::string & entryId, const itk::GDCMImageIO::Pointer dicomIO);
    void SentSignalToTag();

private:
    Ui::MainWindow *ui;
    //TagDialog *externalTagDialog;
    vtkSmartPointer<vtkGDCMImageReader> readerDCMSeries;
    vtkSmartPointer<vtkImageViewer2> imageViewerDCMSeriesX;
    vtkSmartPointer<vtkImageShiftScale> imageShiftScale;
    vtkSmartPointer<vtkStringArray> arrayS;
    vtkSmartPointer<vtkImageLuminance> convertITKLuminanceFilter;
    vtkSmartPointer<vtkImageData> readImageData;
    //vtkImageToImageType::Pointer vtkImageToImageFilter;

    typedef itk::Image<double,2> ImageType;
    typedef itk::VTKImageToImageFilter<ImageType> VTKImageToImageType;
    typedef itk::ImageFileReader<ImageType> ReaderType;

    int mMinSliderX;
    int mMaxSliderX;

signals:
    void SentSignal(std::string,std::string);
};

#endif // MAINWINDOW_H
