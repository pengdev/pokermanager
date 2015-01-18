#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    //set range for money won slider(between 0 and 100)
    ui->money->setRange(0,100);

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(openFile()));
    //connection in general page
    connect(ui->player,SIGNAL(activated(QString)),this,SLOT(tableChanged(QString)));
    connect(ui->player,SIGNAL(activated(QString)),this,SLOT(tableUpdate(QString)));

    //connection in hands page
    connect(ui->cards,SIGNAL(activated(QString)),this,SLOT(handsChanged(QString)));
    connect(ui->tableView_2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(sessionHands(QModelIndex)));

    //connection in sessions page
    connect(ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(dateShow()));
    connect(ui->dateEdit_2,SIGNAL(dateChanged(QDate)),this,SLOT(dateShow()));
    connect(ui->player,SIGNAL(activated(QString)),this,SLOT(dateShow()));
    connect(ui->tableView_6,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(sessionDate(QModelIndex)));

    //connection in money won page
    connect(ui->money,SIGNAL(valueChanged(int)),ui->label_money,SLOT(setNum(int)));
    connect(ui->money,SIGNAL(valueChanged(int)),this,SLOT(moneyChanged()));
    connect(ui->player,SIGNAL(activated(QString)),this,SLOT(moneyChanged()));
    connect(ui->tableView_8,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(sessionMoney(QModelIndex)));


    //connection in positions page
    connect(ui->position,SIGNAL(activated(QString)),this,SLOT(positionChanged(QString)));
    connect(ui->tableView_4,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(sessionPosition(QModelIndex)));

    //settings of statistics table in general page
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QObject::tr("Hands played")
                                               <<QObject::tr("Money won")<<QObject::tr("VPIP %")
                                               <<QObject::tr("PFR %")<<QObject::tr("PF3bet %")
                                               <<QObject::tr("AF %")
                                               <<QObject::tr("Cont. bet flop %")
                                               <<QObject::tr("WTSD %")
                                               <<QObject::tr("W$SD %"));

    //settings of statistics table in sessions page
    ui->tableWidget_3->setColumnCount(9);
    ui->tableWidget_3->setRowCount(1);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setHorizontalHeaderLabels(QStringList()<<QObject::tr("Hands played")
                                               <<QObject::tr("Money won")<<QObject::tr("VPIP %")
                                               <<QObject::tr("PFR %")<<QObject::tr("PF3bet %")
                                               <<QObject::tr("AF %")
                                               <<QObject::tr("Cont. bet flop %")
                                               <<QObject::tr("WTSD %")
                                               <<QObject::tr("W$SD %"));

    //settings of statistics table in hands page
    ui->tableWidget_6->setColumnCount(3);
    ui->tableWidget_6->setRowCount(325);
    ui->tableWidget_6->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_6->setHorizontalHeaderLabels(QStringList()<<QObject::tr("Starting Hands")
                                               <<QObject::tr("Times")<<QObject::tr("Win %"));

    //settings of statistics table in positions page
    ui->tableWidget_7->setColumnCount(4);
    ui->tableWidget_7->setRowCount(2);
    ui->tableWidget_7->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_7->setHorizontalHeaderLabels(QStringList()<<QObject::tr("Times")
                                               <<QObject::tr("VPIP %")<<QObject::tr("PFR %")<<QObject::tr("WTSD %"));
    ui->tableWidget_7->setVerticalHeaderLabels(QStringList()<<QObject::tr("SB")
                                               <<QObject::tr("BB"));


}

void MainWindow::openFile()
{
    QString filename=QFileDialog::getOpenFileName(this,"Open Document",QDir::currentPath(),"Document files(*.txt);;All files(*.*)");
    ui->label_file->setText(filename);
//    if(db.isOpen())
            db.close();
    loadhistory(filename.toStdString());
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Database.sqlite");
    if(!db.open())
    {
         QMessageBox::critical(0,qApp->tr("Cannot open database"),qApp->tr("Unable to establish a databse connection\n"
                                                                          "Click cancel to exit."),QMessageBox::Cancel);
    }

    QSqlQueryModel *player_model= new QSqlQueryModel(ui->player);
    player_model->setQuery(QString("SELECT DISTINCT player FROM a ORDER BY player;"));
    ui->player->setModel(player_model);

    QSqlQueryModel *position_model= new QSqlQueryModel(ui->position);
    position_model->setQuery(QString("SELECT DISTINCT position FROM a ORDER BY position;"));
    ui->position->setModel(position_model);


    QSqlQueryModel *cards_model= new QSqlQueryModel(ui->cards);
    cards_model->setQuery(QString("SELECT DISTINCT cards FROM a ORDER BY cards;"));
    ui->cards->setModel(cards_model);

    table = new QSqlTableModel(ui->tableView,db);    

    table_2 = new QSqlTableModel(ui->tableView_2,db);

    table_4 = new QSqlTableModel(ui->tableView_4,db);

    table_6 = new QSqlTableModel(ui->tableView_6,db);    

    table_8 = new QSqlTableModel(ui->tableView_8,db);


}

void MainWindow::tableChanged(QString n)
{
    table->setTable("a");
    table->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
    table->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
    table->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
    table->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
    table->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
    table->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
    table->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
    table->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
    table->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));
    table->setFilter(QString("player='"+n+"'ORDER BY player;"));
    table->select();
    ui->tableView->setModel(table);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlQueryModel *cards_model= new QSqlQueryModel(ui->cards);
    cards_model->setQuery(QString("SELECT DISTINCT cards FROM a WHERE player='"+n+"'ORDER BY cards;"));
    ui->cards->setModel(cards_model);

    QSqlQueryModel *position_model= new QSqlQueryModel(ui->position);
    position_model->setQuery(QString("SELECT DISTINCT position FROM a WHERE player='"+n+"' ORDER BY position;"));
    ui->position->setModel(position_model);

}

void MainWindow::tableUpdate(QString n)
{
    std::string name=n.toStdString();
    Player player(name);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(QString::number(player.hands_played(""))));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(QString::number(player.money_won("")[player.hands_played("")])));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem(QString::number(player.vpip(""))));
    ui->tableWidget->setItem(0,3,new QTableWidgetItem(QString::number(player.pfr(""))));
    ui->tableWidget->setItem(0,4,new QTableWidgetItem(QString::number(player.threeBet(""))));
    ui->tableWidget->setItem(0,5,new QTableWidgetItem(QString::number(player.agression_factor())));
    ui->tableWidget->setItem(0,6,new QTableWidgetItem(QString::number(player.cbet(""))));
    ui->tableWidget->setItem(0,7,new QTableWidgetItem(QString::number(player.wtsd(""))));
    ui->tableWidget->setItem(0,8,new QTableWidgetItem(QString::number(player.wsd(""))));

    vector<vector<string> > cards = player.cards();
    for(size_t i=0; i<cards.size();++i)
    {
        vector<string> row=cards[i];
        ui->tableWidget_6->setItem(i,0,new QTableWidgetItem(QString::fromStdString(row.at(1))));
        ui->tableWidget_6->setItem(i,1,new QTableWidgetItem(QString::fromStdString(row.at(0))));
        ui->tableWidget_6->setItem(i,2,new QTableWidgetItem(QString::fromStdString(row.at(2))));
    }

    ui->tableWidget_7->setItem(0,0,new QTableWidgetItem(QString::number(player.blind(0))));
    ui->tableWidget_7->setItem(1,0,new QTableWidgetItem(QString::number(player.blind(1))));
    ui->tableWidget_7->setItem(0,1,new QTableWidgetItem(QString::number(player.vpipB(0))));
    ui->tableWidget_7->setItem(1,1,new QTableWidgetItem(QString::number(player.vpipB(1))));
    ui->tableWidget_7->setItem(0,2,new QTableWidgetItem(QString::number(player.pfrB(0))));
    ui->tableWidget_7->setItem(1,2,new QTableWidgetItem(QString::number(player.pfrB(1))));
    ui->tableWidget_7->setItem(0,3,new QTableWidgetItem(QString::number(player.wsdB(0))));
    ui->tableWidget_7->setItem(1,3,new QTableWidgetItem(QString::number(player.wsdB(1))));

     // for the money plot, firstly write to a file
     std::ofstream fs(("./"+name).c_str(), ios::trunc);
     if(!fs)
     {
         std::cerr<<"Cannot open the output file."<<std::endl;
         return;
     }
     vector<double> temp = player.money_won("");
     for (size_t i = 0; i < temp.size(); i++)
         fs << temp[i] << std::endl;
     fs.close();
        //write command to a file
     std::ofstream fs1("gnuplotcmd", ios::trunc);
     if(!fs1)
     {
         std::cerr<<"Cannot open the output file."<<std::endl;
         return;
      }
     fs1 << "set xlabel 'Hands'"<<std::endl;
     fs1 << "set ylabel 'Money Won'"<<std::endl;
     fs1 << "set grid"<<std::endl;
     fs1 << "set terminal png" << std::endl;
     fs1 << "set output '" +name+".png'" << std::endl;
     fs1 << "plot \"" +name+"\" with lines"<< std::endl;
     fs1 << "exit" << std::endl;
     fs1.close();

     if(system("gnuplot < gnuplotcmd"))
         cout<<"WARNING: GNUplot failed"<<endl;

     QString image="./"+n+".png";
     ui->label_img->setPixmap(QPixmap(image));

}

void MainWindow::handsChanged(QString n)
{
    table_2->setTable("a");
    table_2->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
    table_2->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
    table_2->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
    table_2->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
    table_2->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
    table_2->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
    table_2->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
    table_2->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
    table_2->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));
    table_2->setFilter(QString("cards='"+n+"'AND player='"+ui->player->currentText()+"';"));
    table_2->select();

    ui->tableView_2->setModel(table_2);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void MainWindow::sessionHands(QModelIndex index)
{
    QSqlTableModel *table_3 = new QSqlTableModel(ui->tableView_3,db);
    table_3->setTable("a");
    table_3->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
    table_3->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
    table_3->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
    table_3->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
    table_3->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
    table_3->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
    table_3->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
    table_3->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
    table_3->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));

    QString session=table_2->record(index.row()).value(1).toString();

    table_3->setFilter(QString("game_id='"+session+"';"));
    table_3->select();
    ui->tableView_3->setModel(table_3);
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::dateShow()
{
 QDate date=ui->dateEdit->date();
 QDate date2=ui->dateEdit_2->date();

 QString begin=date.toString("yyyy-MM-dd 00:00:00");
 QString end=date2.toString("yyyy-MM-dd 23:59:59");
 table_6->setTable("a");
 table_6->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
 table_6->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
 table_6->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
 table_6->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
 table_6->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
 table_6->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
 table_6->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
 table_6->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
 table_6->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));
 table_6->setFilter(QString("game_date>'"+begin+"'AND game_date <'"+end+"'AND player='"+ui->player->currentText()+"' ORDER BY game_id ASC;"));
 table_6->select();
 ui->tableView_6->setModel(table_6);
 ui->tableView_6->setSelectionBehavior(QAbstractItemView::SelectRows);
 ui->tableView_6->setEditTriggers(QAbstractItemView::NoEditTriggers);

 std::string name=ui->player->currentText().toStdString();
 Player player(name);
 std::string filter="game_date>'"+begin.toStdString()+"'AND game_date <'"+end.toStdString()+"'";
 ui->tableWidget_3->setItem(0,0,new QTableWidgetItem(QString::number(player.hands_played(filter))));
 ui->tableWidget_3->setItem(0,1,new QTableWidgetItem(QString::number(player.money_won(filter)[player.hands_played(filter)])));
 ui->tableWidget_3->setItem(0,2,new QTableWidgetItem(QString::number(player.vpip(filter))));
 ui->tableWidget_3->setItem(0,3,new QTableWidgetItem(QString::number(player.pfr(filter))));
 ui->tableWidget_3->setItem(0,4,new QTableWidgetItem(QString::number(player.threeBet(filter))));
 ui->tableWidget_3->setItem(0,5,new QTableWidgetItem(QString::number(player.agression_factor())));
 ui->tableWidget_3->setItem(0,6,new QTableWidgetItem(QString::number(player.cbet(filter))));
 ui->tableWidget_3->setItem(0,7,new QTableWidgetItem(QString::number(player.wtsd(filter))));
 ui->tableWidget_3->setItem(0,8,new QTableWidgetItem(QString::number(player.wsd(filter))));

}

void MainWindow::sessionDate(QModelIndex index)
{
    QSqlTableModel *table_7 = new QSqlTableModel(ui->tableView_7,db);
    table_7->setTable("a");
    table_7->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
    table_7->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
    table_7->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
    table_7->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
    table_7->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
    table_7->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
    table_7->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
    table_7->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
    table_7->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));

    QString session=table_6->record(index.row()).value(1).toString();

    table_7->setFilter(QString("game_id='"+session+"';"));
    table_7->select();
    ui->tableView_7->setModel(table_7);
    ui->tableView_7->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::moneyChanged()
{
    int num=ui->money->value();
    QString query=QString::number(num);
    table_8->setTable("a");
    table_8->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
    table_8->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
    table_8->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
    table_8->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
    table_8->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
    table_8->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
    table_8->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
    table_8->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
    table_8->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));
    table_8->setFilter(QString("action= 'win' AND player='" + ui->player->currentText() + "'AND money > '" + query + "' ORDER BY game_id"));
    table_8->select();
    ui->tableView_8->setModel(table_8);
    ui->tableView_8->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_8->setEditTriggers(QAbstractItemView::NoEditTriggers);

    std::string name=ui->player->currentText().toStdString();
    Player player(name);
    std::string filter="action= 'win' AND money > "+query.toStdString()+"";
}

void MainWindow::sessionMoney(QModelIndex index)
{
    QSqlTableModel *table_9 = new QSqlTableModel(ui->tableView_9,db);
    table_9->setTable("a");
    table_9->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
    table_9->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
    table_9->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
    table_9->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
    table_9->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
    table_9->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
    table_9->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
    table_9->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
    table_9->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));

    QString session=table_8->record(index.row()).value(1).toString();

    table_9->setFilter(QString("game_id='"+session+"';"));
    table_9->select();
    ui->tableView_9->setModel(table_9);
    ui->tableView_9->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void MainWindow::positionChanged(QString n)
{
    table_4->setTable("a");
    table_4->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
    table_4->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
    table_4->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
    table_4->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
    table_4->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
    table_4->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
    table_4->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
    table_4->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
    table_4->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));
    table_4->setFilter(QString("position='"+n+"'AND player='"+ui->player->currentText()+"';"));
    table_4->select();

    ui->tableView_4->setModel(table_4);
    ui->tableView_4->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void MainWindow::sessionPosition(QModelIndex index)
{
    QSqlTableModel *table_5 = new QSqlTableModel(ui->tableView_5,db);
    table_5->setTable("a");
    table_5->setHeaderData(0,Qt::Horizontal,QObject::tr("Date"));
    table_5->setHeaderData(1,Qt::Horizontal,QObject::tr("Session"));
    table_5->setHeaderData(2,Qt::Horizontal,QObject::tr("Table No."));
    table_5->setHeaderData(3,Qt::Horizontal,QObject::tr("Action"));
    table_5->setHeaderData(4,Qt::Horizontal,QObject::tr("Phase"));
    table_5->setHeaderData(5,Qt::Horizontal,QObject::tr("Private Cards"));
    table_5->setHeaderData(6,Qt::Horizontal,QObject::tr("Player"));
    table_5->setHeaderData(7,Qt::Horizontal,QObject::tr("Money"));
    table_5->setHeaderData(8,Qt::Horizontal,QObject::tr("Position"));

    QString session=table_4->record(index.row()).value(1).toString();

    table_5->setFilter(QString("game_id='"+session+"';"));
    table_5->select();
    ui->tableView_5->setModel(table_5);
    ui->tableView_5->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}
