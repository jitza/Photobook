#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include "imagecollection.h"

/* Name: Jenny Itza
 * ID: 2020152085
   Date: 4/8/2022
   Course Code: CMPS2212-GUI
   Program Description: Creating a photoBook with CRUD functionality
   Source to download qt mysql drivers: https://github.com/thecodemonkey86/qt_mysql_driver/releases
   Additional Info: Inorder to add a image it has to be 65535 bytes (64 KB) maximum.
   --QT += core gui sql-- Add this to the .pro file have access to the QSqlDatabase
*/

namespace Ui {
class ImageDialog;
}

class ImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageDialog(QWidget *parent = nullptr);
    ~ImageDialog();

private slots:
    void nextClicked();             //Move between images
    void previousClicked();         //Move between images
    void tagsChanged();             //Change the selection in the list of tags
    void addImageClicked();         //Add a new image
    void addTagClicked();           //Add a new tag
    void updateTagClicked();        //Updates a tag
    void removeTagClicked();        //Deletes a tag
    void removeImageClicked();      //Deletes an image along with its tags

private:
    QStringList selectedTags();     //Support methods
    void updateImages();
    void updateTags();
    void updateCurrentImage();

    Ui::ImageDialog *ui;

    QList<int> imageIds;            //Contains the id values for the images that are shown according to the selected tags
    int currentImage;               //Is an index into the imageIds that indicates which image is active
    ImageCollection images;         //Image collecton instance that handles the database

};

#endif // IMAGEDIALOG_H
