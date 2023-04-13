#ifndef IMAGECOLLECTION_H
#define IMAGECOLLECTION_H

#include <QImage>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QBuffer>
#include <QImageWriter>
#include <QImageReader>

/* Name: Jenny Itza
 * ID: 2020152085
   Date: 4/8/2022
   Course Code: CMPS2212-GUI
   Program Description: Creating a photoBook with CRUD functionality
   Source to download qt mysql drivers: https://github.com/thecodemonkey86/qt_mysql_driver/releases
   Additional Info: Inorder to add a image it has to be 65535 bytes (64 KB) maximum.
   --QT += core gui sql-- Add this to the .pro file have access to the QSqlDatabase
*/

class ImageCollection
{
public:
    ImageCollection();            //Default constructor
    QImage getImage(int id);
    QList<int> getIds(QStringList tags);
    QStringList getTags();
    void addTag(int id, QString tag);
    void addImage(QImage image, QStringList tags);
    void updateTag(int id, QString newTag, QString oldTag);
    void removeTag(int id, QString tag);
    void removeImage(int id);
private:
    void populateDatabase();
    QSqlDatabase db;
    bool connected;
};

#endif // IMAGECOLLECTION_H
