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
    setWindowTitle("BMI检测器");

    // 设置列数
    ui->tableWidget->setColumnCount(2);

    // 设置表头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "分类" << "BMI 范围");

    // 设置行数
    ui->tableWidget->setRowCount(4);

    // 设置内容
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("偏瘦"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("<= 18.4"));

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("正常"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("18.5 ~ 23.9"));

    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("过重"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("24.0 ~ 27.9"));

    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("肥胖"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem(">= 28.0"));

    // 设置表名
    ui->tableWidget->setWindowTitle("BMI 中国标准");

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
        QMessageBox::warning(this, "警告", "身高和体重未填写！");
        return;
    }
    else if (ui->lineEdit_2->text() == "")
    {
        QMessageBox::warning(this, "警告", "身高未填写！");
        return;
    }
    else if (ui->lineEdit_3->text() == "")
    {
        QMessageBox::warning(this, "警告", "体重未填写！");
        return;
    }

    QString output="您的身高为: "+ui->lineEdit_2->text()+"\n您的体重为: "+ui->lineEdit_3->text();

    if (ui->lineEdit->text() == "")
    {
        QMessageBox::about(this, "您好", output);
    }
    else
    {
        QMessageBox::about(this,ui->lineEdit->text()+"您好", output);
    }

}

const QStringList Words={"偏瘦!这么瘦，多吃一点！","正常！自律！","过重!小问题，微胖。","肥胖！在唐朝多少是个贵妃！"};

void MainWindow::on_pushButton_2_clicked()
{
    // 获取用户输入的身高和体重
    QString heightStr = ui->lineEdit_2->text();
    QString weightStr = ui->lineEdit_3->text();

    // 验证输入是否为空
    if (heightStr.isEmpty() && weightStr.isEmpty())
    {
        QMessageBox::warning(this, "警告", "身高和体重未填写！");
        return;
    }
    else if (heightStr.isEmpty())
    {
        QMessageBox::warning(this, "警告", "身高未填写！");
        return;
    }
    else if (weightStr.isEmpty())
    {
        QMessageBox::warning(this, "警告", "体重未填写！");
        return;
    }

    // 转换身高和体重为浮点数
    bool conversionOk;
    float height = heightStr.toFloat(&conversionOk);
    float weight = weightStr.toFloat(&conversionOk);

    if (!conversionOk || height <= 0 || weight <= 0)
    {
        QMessageBox::warning(this, "警告", "请输入有效的身高和体重！");
        return;
    }



    //欺骗自己
    // 生成随机数
    int random_num = QRandomGenerator::global()->bounded(10)+10; // 生成0到20之间的随机数
    if(ui->checkBox->isChecked())
    {
        weight-=random_num;
    }


    // 将身高从厘米转换为米
    height /= 100.0; // 1米 = 100厘米

    // 计算BMI
    float bmi = weight / (height * height);

    // 根据BMI值生成输出字符串
    QString output = "您好!\n您的身高为: " + heightStr +
            "\n您的体重为: " + weightStr +"(-"+QString::number(random_num)+")"+
            "\n您的BMI值为: " + QString::number(bmi)+"\n\n";

    showImage(bmi,&output);

    ui->listWidget->clear();
    // 添加到列表框中
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
    // 创建一个QMap来存储BMI范围与图片路径的映射
    QMap<int, QString> imageMap;
    imageMap[0] = ui->checkBox_2->isChecked() ? ":/images/man1.jpg" : ":/images/woman1.jpg";
    imageMap[1] = ui->checkBox_2->isChecked() ? ":/images/man5.jpg" : ":/images/woman2.jpg";
    imageMap[2] = ui->checkBox_2->isChecked() ? ":/images/man3.jpg" : ":/images/woman3.jpg";
    imageMap[3] = ui->checkBox_2->isChecked() ? ":/images/man4.jpg" : ":/images/woman4.jpg";

    QString imagePath;

    // 判断BMI范围并设置相应的文本和图片
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

    // 创建一个QPixmap对象并加载图片
    QPixmap pixmap(imagePath);

    // 设置QLabel的Pixmap以显示图片
    ui->label_4->setPixmap(pixmap);
    ui->label_4->setFixedSize(pixmap.size()); // 手动设置大小

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
