#include "downloaderui.h"
#include "ui_downloaderui.h"

DownLoaderUI::DownLoaderUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownLoaderUI)
{
   // ui->setupUi(this);
    createTaskUI = new CreateTaskUI();


    addTaskWindows();
    settingUI();


}
DownLoaderUI::~DownLoaderUI()
{
    delete ui;
    delete taskListView;
    delete createTaskUI;

}
void DownLoaderUI::settingUI() {
    taskListView->setObjectName("taskListVeiw");
    add_button->setObjectName("buttonAdd");
    start_button->setObjectName("buttonStart");
    pause_button->setObjectName("buttonPause");
    remove_button->setObjectName("buttonRemove");
    add_button->setMinimumSize(QSize(85,30));
    start_button->setMinimumSize(QSize(60,30));
    pause_button->setMinimumSize(QSize(60,30));
    remove_button->setMinimumSize(QSize(60,30));
}

void DownLoaderUI::addTaskWindows() {

    taskListView = new TaskListView(this);


    add_button = new QPushButton(this);

    start_button = new QPushButton(this);
    pause_button = new QPushButton(this);
    remove_button = new QPushButton(this);
    QHBoxLayout *operateLayout = new QHBoxLayout();

    operateLayout->addWidget(add_button);
    operateLayout->addWidget(start_button);
    operateLayout->addWidget(pause_button);
    operateLayout->addWidget(remove_button);
    operateLayout->addStretch();
    operateLayout->setSpacing(2);
    QVBoxLayout *main_layout = new QVBoxLayout();

    main_layout->addLayout(operateLayout);
    main_layout->addWidget(taskListView);
    main_layout->setSpacing(0);

    main_layout->setContentsMargins(0,0, 0,0);

    this->setLayout(main_layout);
    //添加新下载任务
    connect(add_button,&QPushButton::clicked,createTaskUI,&CreateTaskUI::show);
    connect(createTaskUI,&CreateTaskUI::createTaskSignal,this,&DownLoaderUI::addTask);

    connect(start_button,&QPushButton::clicked,this,&DownLoaderUI::startTask);
    connect(pause_button,&QPushButton::clicked,this,&DownLoaderUI::pauseTask);
    connect(remove_button,&QPushButton::clicked,this,&DownLoaderUI::removeTask);
}


//更新下载任务列表显示
void DownLoaderUI::updateTaskView(DownloadTaskStatus taskList)
{

    QList<QStringList> grid;
    QList<DownloadTask> &tasks = taskList.tasks;

    // <pid,pid_state>,
    QMap<QString,QString> task;
    //<id>
    QVector<QString> locate;
    for(int i = 0;i<tasks.size();i++) {
        QStringList row;
        DownloadTask item = tasks.at(i);



        row<<""<<item.name   //名称
           <<perfectSize(item.size) //大小
           <<QString::number((double)item.progress/100) //进度
           <<perfectRemainTime(item.remainTime)//剩余时间

           <<perfectSize(item.speed)+"/s"//速度
           <<perfectState(item.state);//剩余时间

        grid.append(row);
        locate.push_back(item.id);
        task[item.id] = item.id+"_"+QString::number(item.state);



    }
    taskListView->updateAllData(&grid,&task,&locate);


}

QString DownLoaderUI::perfectSize(QVariant value) {
    const static double B  = 1024;
    const static double KB = 1024*1024;
    const static double MB = 1024*1024*1024;
   // const static double GB = 1024*1024*1024*1024;
    double v=value.toDouble();
    if(v<B) {
        return QString::number(v)+"B";
    }else if(v<KB) {

        double t_double = v/B;

        int t_int = t_double*100;
        t_double = (double)t_int/100; //取两位整数

        return QString::number(t_double)+"KB";
    } else if(v < MB) {
        double t_double = v / KB;
        int t_int = t_double*100;
        t_double = (double)t_int/100;
        return QString::number(t_double)+"MB";
    } else {
        double t_double = v/MB;
        quint64 t_int = t_double*100;
        t_double = (double)t_int/100;
        return QString::number(t_double)+"GB";
    }

}
QString DownLoaderUI::perfectState(int state) {
 /*LIST_TASK_STATE_DOWNLOADING: 0,
 LIST_TASK_STATE_WAITING: 8,
 LIST_TASK_STATE_STOPPED: 9,
 LIST_TASK_STATE_PAUSED: 10,
 LIST_TASK_STATE_COMPLETED: 11,
 LIST_TASK_STATE_FAILED: 12,
 LIST_TASK_STATE_UPLOADING: 13,
 LIST_TASK_STATE_PREDOWNLOAD: 14,
 LIST_TASK_STATE_DELETED: 15,
 LIST_TASK_STATE_RECYCLED: 16,
 LIST_TASK_STATE_PENDING: 37,
 LIST_TASK_STATE_ERROR: 38,
   */

    switch(state) {
    case 0:
        return "正在下载";
    case 8:
        return "等待";
    case 9:
        return "暂停";
    case 10:
        return "暂停";
    case 11:
        return "完成";
    case 12:
        return "失败";
    case 13:
        return "正在上传";
    case 14:
        return "等待插入外接存储设备";
    case 15:
        return "已删除";
    case 37:
        return "准备下载";
    case 38:
        return "错误";
    default:
        return "";

    }
}
QString DownLoaderUI::perfectRemainTime(quint64 value)
{
    QString timeStr;
    if(value==0)
        return "--:--:--";
    int hour=value/3600;
    value = value%3600;
    int minute = value/60;
    int second = value%60;

    char buf[1024];

    ::snprintf(buf, 1024, "%02d:%02d:%02d", hour,minute,second);
    timeStr = QString::fromUtf8(buf);

    return timeStr;

}

extern QString downloaderPID;
void DownLoaderUI::addTask(QString url, QString taskName)
{
    QString args="/createTask?pid="+downloaderPID;
    emit add_signal(args,url,taskName);
    qDebug()<<"create New Task";

}
void DownLoaderUI::startTask() {
    QString args="/start?pid="+downloaderPID+"&tasks=";
    args +=taskListView->getCheckBoxSelect();
    emit start_signal(args);
    qDebug()<<"DownLoaderUI::startTask()"<<args;

}

void DownLoaderUI::pauseTask() {
    QString args="/pause?pid="+downloaderPID+"&tasks=";
    args +=taskListView->getCheckBoxSelect();
    emit pause_signal(args);
    qDebug()<<"DownLoaderUI::pauseTask()"<<args;
}
void DownLoaderUI::removeTask() {
    QString args = "/del?recycleTask=1&deleteFile=true&pid="
            +downloaderPID+"&tasks=";
    args +=taskListView->getCheckBoxSelect();
    emit remove_signal(args);
    qDebug()<<"DownLoaderUI::removeTask"<<args;
}

void DownLoaderUI::createTaskResult(const QString &info)
{
    infoDialog.setWindowTitle(tr("创建任务结果"));
    infoDialog.resize(200,60);

    infolabel.setText(info);

    infoLayout.addWidget(&infolabel);
    infoDialog.setLayout(&infoLayout);

    infoDialog.setWindowFlags(Qt::FramelessWindowHint);
    infoDialog.move(800,90);
    infoDialog.show();

    QTimer::singleShot(2000, &infoDialog, SLOT(close()));


}

void DownLoaderUI::test() {


}
