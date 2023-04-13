#include "imagedialog.h"
#include "ui_imagedialog.h"

ImageDialog::ImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageDialog)
{
    ui->setupUi(this);
        currentImage = -1;   //Initailizes currentImage to an invalid value

        updateTags();
        updateImages();

        //Connectors
        connect(ui->previousButton, SIGNAL(clicked()), this, SLOT(previousClicked()));
        connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(nextClicked()));
        connect(ui->addTagButton, SIGNAL(clicked()), this, SLOT(addTagClicked()));
        connect(ui->addImageButton, SIGNAL(clicked()), this, SLOT(addImageClicked()));
        connect(ui->updateTagButton, SIGNAL(clicked()), this, SLOT(updateTagClicked()));
        connect(ui->removeTagButton, SIGNAL(clicked()), this, SLOT(removeTagClicked()));
        connect(ui->removeImageButton, SIGNAL(clicked()), this, SLOT(removeImageClicked()));
        connect(ui->tagList, SIGNAL(itemSelectionChanged()), this, SLOT(tagsChanged()));

}

ImageDialog::~ImageDialog()
{
    delete ui;
}

void ImageDialog::nextClicked()
{
    currentImage = (currentImage + 1) % imageIds.count(); //Index is increased
    updateCurrentImage();
}

void ImageDialog::previousClicked()
{
    currentImage--;                                       //Index is decreased

    if(currentImage == -1)
        currentImage = imageIds.count() -1;               //Starting from the other end

    updateCurrentImage();
}

void ImageDialog::tagsChanged()
{
    updateImages();                                       //Getting new list of images
}

void ImageDialog::addImageClicked()
{
    QString filename = QFileDialog::getOpenFileName(      //Making user to add an image
                this, tr("Open file"), QString(), tr("PNG Images (*.png)"));
    if(!filename.isNull())
    {
        QImage image(filename);
        if(image.isNull())
        {
            QMessageBox::warning(this, tr("Image Book"),
                                 tr("Failed to open the file '%1'").arg(filename));
            return;
        }
        images.addImage(image, selectedTags());  //Adding image along with the selected tag to the image collection object
        updateImages();
    }
}

void ImageDialog::addTagClicked()
{
    bool ok;
    QString tag = QInputDialog::getText(this, tr("Photo Book"),            //Asking user for a tag
                                        tr("Tag:"), QLineEdit::Normal, QString(), &ok);
    if(ok)
    {
        tag = tag.toLower();   //Convert input to lower case
        QRegularExpression re(QRegularExpression::anchoredPattern("[a-z]+"));
        QRegularExpressionMatch match = re.match(tag);    //Verifying input

        if(!match.hasMatch())
        {
            QMessageBox::warning(this, tr("Image Book"), tr("This is not a valid tag."
                                                            "Tags consists of lower case characters a-z."));
            return;
        }
        images.addTag(imageIds[currentImage], tag); //Adding tag to current image
        updateTags();
    }
}

void ImageDialog::updateTagClicked()
{
   QString selectedTag;                      //Storing the selected tag

   foreach(QString tag, selectedTags()){     //Getting selected tag
       selectedTag = tag;
   }

   if(selectedTag.isNull()){
       QMessageBox::warning(this, tr("Image Book"), tr("Can not update a tag, if there is no tag selected."));
       return;
   }

   bool ok;                                 //Asking user for the updated tag
   QString newTag = QInputDialog::getText(this, tr("Photo Book"),
                                       tr("Updating tag: '%1'").arg(selectedTag), QLineEdit::Normal, QString(), &ok);
   if(ok)
   {
       newTag = newTag.toLower();   //Convert input to lower case
       QRegularExpression re(QRegularExpression::anchoredPattern("[a-z]+"));
       QRegularExpressionMatch match = re.match(newTag);    //Verifying input

       if(!match.hasMatch())
       {
           QMessageBox::warning(this, tr("Image Book"), tr("This is not a valid tag."
                                                           "Tags consists of lower case characters a-z."));
           return;
       }
       images.updateTag(imageIds[currentImage], newTag, selectedTag);
       updateTags();
   }
}

void ImageDialog::removeTagClicked()
{
    QString selectedTag;

    foreach(QString tag, selectedTags()){
        selectedTag = tag;
    }

    if(selectedTag.isNull()){
        QMessageBox::warning(this, tr("Image Book"), tr("Can not remove tag, if there is no tag selected."));
        return;
    }
    switch(QMessageBox::warning (this, tr("Image Book"),         //Asking user if he wants to remove the tag he selected
                              tr("Delete Tag: '%1' for this image?").arg(selectedTag),
                                 QMessageBox::Yes | QMessageBox::No)){

        case QMessageBox::Yes:           //If yes the tag is removed
               images.removeTag(imageIds[currentImage], selectedTag);
               updateTags();
            break;

        case QMessageBox::No:
                return;
            break;

        default:
            break;
    }
}

void ImageDialog::removeImageClicked()
{
    if(currentImage == -1){
        QMessageBox::warning(this, tr("Image Book"), tr("No image available to be removed"));
        return;
    }
    switch(QMessageBox::warning (this, tr("Image Book"),    //Asking user if he wants to delete the current image
                          tr("Are you sure you want to delete this image?"),QMessageBox::Yes | QMessageBox::No)){

    case QMessageBox::Yes:                //If yes the image is removed
           images.removeImage(imageIds[currentImage]);
           updateImages();
           updateTags();
        break;

    case QMessageBox::No:
            return;
        break;

    default:
        break;
    }
}

QStringList ImageDialog::selectedTags()
{
    QStringList result;

    foreach(QListWidgetItem * item, ui->tagList->selectedItems()) //Iterating through the items in the list widget
        result << item->text();

    return result; //returning the selected item
}

void ImageDialog::updateImages()
{
    int id;

    if(currentImage != -1)                 //Checking if an image is available
        id = imageIds[currentImage];
    else
        id=-1;
    imageIds = images.getIds(selectedTags());      //Getting a new list of image id values from the ImageCollection based on the current selection of tags
    currentImage = imageIds.indexOf(id);

    if(currentImage == -1 && !imageIds.isEmpty())
        currentImage = 0;
    ui->imagesLabel->setText(QString::number(imageIds.count()));

    updateCurrentImage();
}

void ImageDialog::updateTags()
{
    QStringList selection = selectedTags();  //Getting current sselected tag

    QStringList tags = images.getTags();     //Getting new set of tags
    ui->tagList->clear();                    //Clearing list of tags
    ui->tagList->addItems(tags);             //Adding new tags

    for(int i = 0; i < ui->tagList->count(); ++i)
        if(selection.contains(ui->tagList->item(i)->text()))
            ui->tagList->item(i)->setSelected(true);
}

void ImageDialog::updateCurrentImage()
{
    if(currentImage == -1)    //If there is no image available
    {
        ui->imageLabel->setPixmap(QPixmap());
        ui->imageLabel->setText(tr("No Image"));

        ui->addTagButton->setEnabled(false);
        ui->nextButton->setEnabled(false);
        ui->previousButton->setEnabled(false);
     }
     else
    {
        ui->imageLabel->setPixmap(    //Getting image from the image collection and showing it
                    QPixmap::fromImage(images.getImage(imageIds[currentImage])));
        ui->addTagButton->setEnabled(true);
        ui->nextButton->setEnabled(true);
        ui->previousButton->setEnabled(true);


    }
}
