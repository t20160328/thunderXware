#include "downloaderui.h"
#include "ui_downloaderui.h"

DownLoaderUI::DownLoaderUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownLoaderUI)
{
    ui->setupUi(this);
    taskListView = new TaskListView();
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addWidget(taskListView);

    main_layout->setSpacing(20);
    main_layout->setContentsMargins(20, 20, 20, 20);

    this->setLayout(main_layout);


}

DownLoaderUI::~DownLoaderUI()
{
    delete ui;
}
//更新下载任务列表显示
void DownLoaderUI::updateTaskView(DownloadTaskStatus taskList)
{
    QList<QStringList> grid;
    QList<DownloadTask> &tasks = taskList.tasks;

    foreach(DownloadTask item,tasks) {
        QStringList row;
        int time=item.remainTime/3600;


        row<<item.name   //名称
           <<perfectSize(item.size) //大小
           <<QString::number((double)item.progress/100) //进度
           <<QString::number(time)//剩余时间

           <<perfectSize(item.speed)+"/s"//速度
           <<perfectState(item.state);//剩余时间
        grid.append(row);

    }
    taskListView->updateAllData(grid);


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
        return "停止";
    case 10:
        return "暂停";
    case 11:
        return "完成";
    case 12:
        return "失败";
    case 13:
        return "正在上传";
    case 14:
        return "准备下载";
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

void DownLoaderUI::test() {


}
