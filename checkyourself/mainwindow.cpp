#include "mainwindow.h"
#include "ui_mainwindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/win.png"));
    setWindowTitle("BMI�����");

    // ��������
    ui->tableWidget->setColumnCount(2);

    // ���ñ�ͷ
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "����" << "BMI ��Χ");

    // ��������
    ui->tableWidget->setRowCount(4);

    // ��������
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("ƫ��"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("<= 18.4"));

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("����"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("18.5 ~ 23.9"));

    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("����"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("24.0 ~ 27.9"));

    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("����"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem(">= 28.0"));

    // ���ñ���
    ui->tableWidget->setWindowTitle("BMI �й���׼");

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->checkBox_2->setCheckState(Qt::Checked);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit_2->text() == "" && ui->lineEdit_3->text() == "")
    {
        QMessageBox::warning(this, "����", "��ߺ�����δ��д��");
        return;
    }
    else if (ui->lineEdit_2->text() == "")
    {
        QMessageBox::warning(this, "����", "���δ��д��");
        return;
    }
    else if (ui->lineEdit_3->text() == "")
    {
        QMessageBox::warning(this, "����", "����δ��д��");
        return;
    }

    QString output="�������Ϊ: "+ui->lineEdit_2->text()+"\n��������Ϊ: "+ui->lineEdit_3->text();

    if (ui->lineEdit->text() == "")
    {
        QMessageBox::about(this, "����", output);
    }
    else
    {
        QMessageBox::about(this,ui->lineEdit->text()+"����", output);
    }

}

const QStringList Words={"ƫ��!��ô�ݣ����һ�㣡","���������ɣ�","����!С���⣬΢�֡�","���֣����Ƴ������Ǹ�������"};

void MainWindow::on_pushButton_2_clicked()
{
    // ��ȡ�û��������ߺ�����
    QString heightStr = ui->lineEdit_2->text();
    QString weightStr = ui->lineEdit_3->text();

    // ��֤�����Ƿ�Ϊ��
    if (heightStr.isEmpty() && weightStr.isEmpty())
    {
        QMessageBox::warning(this, "����", "��ߺ�����δ��д��");
        return;
    }
    else if (heightStr.isEmpty())
    {
        QMessageBox::warning(this, "����", "���δ��д��");
        return;
    }
    else if (weightStr.isEmpty())
    {
        QMessageBox::warning(this, "����", "����δ��д��");
        return;
    }

    // ת����ߺ�����Ϊ������
    bool conversionOk;
    float height = heightStr.toFloat(&conversionOk);
    float weight = weightStr.toFloat(&conversionOk);

    if (!conversionOk || height <= 0 || weight <= 0)
    {
        QMessageBox::warning(this, "����", "��������Ч����ߺ����أ�");
        return;
    }



    //��ƭ�Լ�
    // ���������
    int random_num = QRandomGenerator::global()->bounded(10)+10; // ����0��20֮��������
    if(ui->checkBox->isChecked())
    {
        weight-=random_num;
    }


    // ����ߴ�����ת��Ϊ��
    height /= 100.0; // 1�� = 100����

    // ����BMI
    float bmi = weight / (height * height);

    // ����BMIֵ��������ַ���
    QString output = "����!\n�������Ϊ: " + heightStr +
            "\n��������Ϊ: " + weightStr +"(-"+QString::number(random_num)+")"+
            "\n����BMIֵΪ: " + QString::number(bmi)+"\n\n";

    showImage(bmi,&output);

    ui->listWidget->clear();
    // ��ӵ��б����
    if (ui->lineEdit->text().isEmpty())
    {
        ui->listWidget->addItem(output);
    }
    else
    {
        ui->listWidget->addItem(ui->lineEdit->text() + output);
    }
}

void MainWindow::showImage(float bmi,QString *out)
{
    // ����һ��QMap���洢BMI��Χ��ͼƬ·����ӳ��
    QMap<int, QString> imageMap;
    imageMap[0] = ui->checkBox_2->isChecked() ? ":/images/man1.jpg" : ":/images/woman1.jpg";
    imageMap[1] = ui->checkBox_2->isChecked() ? ":/images/man5.jpg" : ":/images/woman2.jpg";
    imageMap[2] = ui->checkBox_2->isChecked() ? ":/images/man3.jpg" : ":/images/woman3.jpg";
    imageMap[3] = ui->checkBox_2->isChecked() ? ":/images/man4.jpg" : ":/images/woman4.jpg";

    QString imagePath;

    // �ж�BMI��Χ��������Ӧ���ı���ͼƬ
    if (bmi <= 18.4)
    {
        *out += Words[0];
        imagePath=imageMap[0];
    }
    else if (bmi <= 23.9)
    {
        *out += Words[1];
        imagePath=imageMap[1];
    }
    else if (bmi <= 27.9)
    {
        *out += Words[2];
        imagePath=imageMap[2];
    }
    else
    {
        *out += Words[3];
        imagePath=imageMap[3];
    }

    // ����һ��QPixmap���󲢼���ͼƬ
    QPixmap pixmap(imagePath);

    // ����QLabel��Pixmap����ʾͼƬ
    ui->label_4->setPixmap(pixmap);
    ui->label_4->setFixedSize(pixmap.size()); // �ֶ����ô�С

}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    bool isCheck = ui->checkBox_2->isChecked();
    if(isCheck==1)
        ui->checkBox_3->setCheckState(Qt::Unchecked);
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    bool isCheck = ui->checkBox_3->isChecked();
    if(isCheck==1)
        ui->checkBox_2->setCheckState(Qt::Unchecked);
}
