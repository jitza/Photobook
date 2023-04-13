#include "imagecollection.h"

ImageCollection::ImageCollection()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
       db.setHostName("127.0.0.1");
       db.setUserName("root");
       db.setPassword("mysql");
       db.setDatabaseName("photoBook");

       if (!db.open()){
          qFatal("Failed to connect to database");
       }
       populateDatabase();
}

QImage ImageCollection::getImage(int id)
{
      QSqlQuery qry;

      qry.prepare( "SELECT data FROM images WHERE id = :id" );
      qry.bindValue( ":id", id );

      if( !qry.exec() )
          qFatal( "Failed to get image from database" );

      if( !qry.next() )
          qFatal( "Failed to get image id from database" );

      QByteArray array = qry.value(0).toByteArray();
      QBuffer buffer(&array);

      buffer.open( QIODevice::ReadOnly );
      QImageReader reader(&buffer, "PNG");
      QImage image = reader.read();

      return image;

}

QList<int> ImageCollection::getIds(QStringList tags)
{
    QSqlQuery qry;

    QList<int> ids;

    if(tags.count() == 0)
        qry.prepare("SELECT images.id FROM images");
    else
        qry.prepare("SELECT id FROM tags WHERE tag IN ('" + tags.join("','") + "') GROUP BY id");

    if (!qry.exec())
      qFatal ("Couldn't get images ids");

    while (qry.next())
    {
        ids<<qry.value(0).toInt();
    }
    return ids;

}

QStringList ImageCollection::getTags()
{
    QSqlQuery query;

    QStringList tags;

    query.prepare("SELECT DISTINCT tag FROM tags");

    if (!query.exec())
      qFatal ("Couldn't get tags");

    while (query.next())
    {
        tags<<query.value(0).toString();
    }
    return tags;
}

void ImageCollection::addTag(int id, QString tag)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tags(id, tag) VALUES (?, ?)");
    query.addBindValue(id);
    query.addBindValue(tag);

    if(!query.exec())
        qFatal("Insertion into tags table Failed");
}

void ImageCollection::addImage(QImage image, QStringList tags)
{
    QBuffer buffer;
    QImageWriter writer(&buffer, "PNG");

    writer.write(image);

    QSqlQuery qry;
    int id = 0;

    qry.prepare("SELECT max(id) FROM images");
    qry.exec();
    while(qry.next()){
        id = qry.value(0).toInt();
    }

    qry.prepare("INSERT INTO images (data) VALUES (:data)");
    qry.bindValue(":data", buffer.data());
    qry.exec();

    foreach(QString tag, tags)
        addTag(id,tag);

}

void ImageCollection::updateTag(int id, QString newTag, QString oldTag)
{
    QSqlQuery qry;

    qry.prepare( "UPDATE tags SET tag=:newTag WHERE id=:id AND tag=:oldTag" );
    qry.bindValue( ":newTag", newTag );
    qry.bindValue( ":id", id );
    qry.bindValue( ":oldTag", oldTag );

    if( !qry.exec() )
        qFatal( "Failed to update tag" );
}

void ImageCollection::removeTag(int id, QString tag)
{
    QSqlQuery qry;

    qry.prepare( "DELETE FROM `tags` WHERE id=:id AND tag=:tag" );
    qry.bindValue( ":id", id );
    qry.bindValue( ":tag", tag );

    if( !qry.exec() )
        qFatal( "Failed to Delete tag" );
}

void ImageCollection::removeImage(int id)
{
    QSqlQuery qry;

    qry.prepare( "DELETE FROM `tags` WHERE id=:id" );
    qry.bindValue( ":id", id );

    if( !qry.exec() )
        qFatal( "Failed to Delete image tags" );

    qry.prepare( "DELETE FROM `images` WHERE id=:id" );
    qry.bindValue( ":id", id );

    if( !qry.exec() )
        qFatal( "Failed to Delete image " );
}

void ImageCollection::populateDatabase()
{
    QSqlQuery qry;

    qry.prepare( "CREATE TABLE IF NOT EXISTS images (id INT PRIMARY KEY AUTO_INCREMENT, data BLOB)" );

    if( !qry.exec())
    qFatal( "Failed to create images table" );

    qry.prepare( "CREATE TABLE IF NOT EXISTS tags (id INT, tag VARCHAR(30))" );

    if( !qry.exec())
    qFatal( "Failed to create tags table" );

}
