#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBuffer>
#include <memory>
#include <QProcess>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <iostream>
#include <QMetaMethod>

MainWindow* MainWindow::pMainWindow = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pMainWindow = this;
    //listView
    listview_qlm = new QStringListModel(this);
//    QStringList listview_ql;
//    listview_ql << "First" << "Second" << "Third";
//    listview_qlm->setStringList(listview_ql);
    ui->listView->setModel(listview_qlm);
    connect(ui->lcdNumber, &MyLCDTime::sendString,
            [=](QString str){ this->AddText2Views(str); });
}

MainWindow::~MainWindow()
{
    delete listview_qlm;
    delete ui;
}

MainWindow& MainWindow::getMainWinPtr()
{
    return *pMainWindow;
}

void MainWindow::on_pushButton_clicked()
{
    exit(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label->setText("GRISHA");
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->label->setText("ULJA");
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->label->setText("TASJA");
}

void MainWindow::on_pushButton_5_clicked()
{
    QApplication::beep();
}

int32_t g_counter1 = 0;

void* my_get_counter()
{
    return &g_counter1;
}

int32_t my_get_counter_str(QString &str)
{
    str = "g_counter1 = " + QString::number(g_counter1);
    return g_counter1;
}

void MainWindow::AddText2Views(QString str)
{
    // Adding at the end

    // Get the position
    int row = listview_qlm->rowCount();

    // Enable add one or more rows
    listview_qlm->insertRows(row,1);

    // Get the row for Edit mode
    QModelIndex index = listview_qlm->index(row);

    listview_qlm->setData(index, str);
    ui->listView->scrollToBottom();
    // Enable item selection and put it edit mode
    //      ui->listView->setCurrentIndex(index);
    //      ui->listView->edit(index);

    //textEdit
//    ui->textEdit->moveCursor(QTextCursor::End);
//    ui->textEdit->insertPlainText("New one\n");
    ui->textEdit->append(str);

//    std::unique_ptr p;

}

void MainWindow::AddText2ViewsS(QString str)
{
    getMainWinPtr().AddText2Views(str);
}

void MainWindow::ClearViews()
{
    listview_qlm->removeRows(0, listview_qlm->rowCount());
    ui->textEdit->clear();

}

void MainWindow::on_pushButton_6_clicked()
{
    g_counter1++;
    ui->label_2->setText("g_counter1 = "+QString::number(g_counter1));
    AddText2Views("string "+QString::number(g_counter1));
}

void MainWindow::on_pushButton_7_clicked()
{
    ClearViews();
}

void MainWindow::on_pushButton_8_clicked()
{
    int32_t* i = (int32_t*)my_get_counter();
    AddText2Views("my_get_counter equal "+QString::number(*i));
}

void my_swap(int32_t& i1, int32_t& i2)
{
    int32_t t = i1;
    i1 = i2;
    i2 = t;
}

void my_swap2(int32_t* i1, int32_t* i2)
{
    int32_t t = *i1;
    *i1 = *i2;
    *i2 = t;
}

void MainWindow::on_pushButton_9_clicked()
{
    int32_t var1 = 10, var2 = 20;
    AddText2Views("(before swap) var1 = "+QString::number(var1)+", var2 = "+QString::number(var2));
    my_swap(var1, var2);
    AddText2Views("(after swap) var1 = "+QString::number(var1)+", var2 = "+QString::number(var2));
    my_swap2(&var1, &var2);
    AddText2Views("(after swap2) var1 = "+QString::number(var1)+", var2 = "+QString::number(var2));

    int32_t x = 1, y = 2;
    int32_t *a = &x;
    int32_t *b = a;
    int32_t *&c = a;
//    a = &y;
    c = &y;
    AddText2Views("a = "+QString::number(*a)+", b = "+QString::number(*b)+", c = "+QString::number(*c));
    QProcess p;
    QStringList args = {"-l", "-a"};
    p.start("ls", args);
    p.waitForFinished();
    QString rez = QString::fromLocal8Bit(p.readAllStandardOutput());
//    foreach(QString str, rez.split("\n")){
//        AddText2Views(str);
//    }
    QStringList lst = rez.split("\n");
    for(QStringList::Iterator it = lst.begin(); it != lst.end(); ++it){
        AddText2Views(*it);
    }
}

QString metamethodaccess(QMetaMethod &mm)
{
    switch ((int)mm.access()) {
    case QMetaMethod::Private:
        return QString("private ");
    case QMetaMethod::Protected:
        return QString("protected ");
    case QMetaMethod::Public:
        return QString("public ");
    }
    return QString("N/A ");
}

QString metamethodtype(QMetaMethod &mm)
{
    switch ((int)mm.methodType()) {
    case QMetaMethod::Method:
        return QString("(method) ");
    case QMetaMethod::Signal:
        return QString("(signal) ");
    case QMetaMethod::Slot:
        return QString("(slot) ");
    case QMetaMethod::Constructor:
        return QString("(constructor) ");
    }
    return QString("(N/A) ");
}

QString metamethodsignature(QMetaMethod &mm)
{
    return QString(mm.methodSignature());
}

void MainWindow::on_pushButton_10_clicked()
{
    QString str;
    my_get_counter_str(str);
    ui->label_3->setText(str);

    /////
    /// \brief moTest
    ///
    const QMetaObject *moTest = ui->centralwidget->metaObject();
    // Start from MyClass members
    for(int methodIdx = moTest->methodOffset(); methodIdx < moTest->methodCount(); ++methodIdx) {
        QMetaMethod mmTest = moTest->method(methodIdx);
        AddText2Views(metamethodaccess(mmTest)+metamethodtype(mmTest)+metamethodsignature(mmTest));
    }
}


class Item : public QObject
{
//Q_OBJECT
public:
    Item(QString nm, MainWindow& w) : Fw(w), name(nm) { Fw.AddText2Views(name+": item acquired"); }
    ~Item() {  Fw.AddText2Views(name+": item destroyed"); }
private:
    MainWindow &Fw;
    QString name;
};

void MainWindow::on_pushButton_11_clicked()
{
//    MainWindow *m = getMainWinPtr();
//    Item *i1 = new Item(getMainWinPtr());
    Item i1("i1", getMainWinPtr());
    Item i2("i2", getMainWinPtr());
    Item i3("i3", *this);

//    Item2 i21("i2_1", getMainWinPtr());
//    Item2 i22("i2_2", getMainWinPtr());
//    Item2 i23("i2_3", *this);
//    Item2 *i24 = new Item2("i2_4");
//    delete i24;
    //    delete i1;
    Item3 i3_1("i3_1", *this), i3_2("i3_2", *this);
//    QObject::connect(&i3_1, SIGNAL(valueChanged(int32_t)), &i3_2, SLOT(setValue(int32_t)));
    QObject::connect(&i3_1, &Item3::valueChanged, &i3_2, &Item3::setValue);
    i3_2.setValue(12);
    QThread::usleep(1000000);
    i3_1.setValue(24);
    QThread::usleep(1000000);
}

void MainWindow::slotShowLogString(QString str)
{
    AddText2Views(str);
}

Item3::Item3(QString name, MainWindow &window) : m_name(name), m_window(window) {
    m_value = 0;
    m_window.AddText2Views(QTime::currentTime().toString("(hh:mm:ss.zzz) ")+m_name+" created");
}

Item3::~Item3()
{
    m_window.AddText2Views(QTime::currentTime().toString("(hh:mm:ss.zzz) ")+m_name+" destroyed");
}

void Item3::setValue(int32_t value){
    if(value!=m_value){
        m_value = value;
        m_window.AddText2Views(QTime::currentTime().toString("(hh:mm:ss.zzz) ")+m_name+": new value is "+QString::number(m_value));
        emit valueChanged(m_value);
    }
}

#include <algorithm>
#include <cmath>

auto abssort(float* x, unsigned n) {
    std::sort(x, x + n,
        // Lambda expression begins
        [](float a, float b) {
            return (std::abs(a) < std::abs(b));
        } // end of lambda expression
    );
}

auto abssort2(std::vector<float> &x) {
    std::sort(x.begin(), x.end(),
        // Lambda expression begins
        [](float a, float b) {
            return (std::abs(a) < std::abs(b));
        } // end of lambda expression
    );
}

//sort button
void MainWindow::on_pushButton_12_clicked()
{
//    float af[10] = {1,5,3,7,9,6,2,0,8,4};
    std::vector<float> vf = {1,5,3,7,9,6,2,0,8,4};
    AddText2Views("Before sort");
#if 1
    int vectoritems_count = 0;
    std::for_each(vf.begin(), vf.end(),
                  [&](const float &v) {
        this->AddText2Views(QString::number(v)); vectoritems_count++; }
    );
    AddText2Views("count elements is "+QString::number(vectoritems_count));

#else
    for(auto v=vf.begin(); v!=vf.end(); ++v){
        AddText2Views(QString::number(*v));
    }
    foreach(auto &v, vf){
        AddText2Views(QString::number(v));
    }
    for(auto &v: vf){
        AddText2Views(QString::number(v));
    }
#endif

//    abssort(&af[0], 10);
    abssort2(vf);
    AddText2Views("After sort");
    for(auto &v: vf){
        AddText2Views(QString::number(v));
    }
}

//lambda
void MainWindow::on_pushButton_13_clicked()
{
    int count_methods_all = 0;

//    std::function listmethods{
//    auto listmethods{
    std::function listmethods{
        [&](const QMetaObject* mo) -> int{
            int count_methods = 0;
            for(int methodIdx = mo->methodOffset(); methodIdx < mo->methodCount(); ++methodIdx) {
                QMetaMethod mmTest = mo->method(methodIdx);
                AddText2Views(metamethodaccess(mmTest)+metamethodtype(mmTest)+metamethodsignature(mmTest));
                count_methods++;
                count_methods_all++;
            }
            return count_methods;
        }
    };

    const QMetaObject* mo = nullptr;

    mo = ui->centralwidget->metaObject();
    auto r = listmethods(mo);
    AddText2Views("--== count methods of "+QString::fromLocal8Bit(mo->className())+" is "+QString::number(r)+" ==--\n");

    mo = ui->lcdNumber->metaObject();
    r = listmethods(mo);
    AddText2Views("--== count methods of "+QString::fromLocal8Bit(mo->className())+" is "+QString::number(r)+" ==--\n");

    mo = ui->label->metaObject();
    r = listmethods(mo);
    AddText2Views("--== count methods of "+QString::fromLocal8Bit(mo->className())+" is "+QString::number(r)+" ==--\n");

    mo = ui->pushButton->metaObject();
    r = listmethods(mo);
    AddText2Views("--== count methods of "+QString::fromLocal8Bit(mo->className())+" is "+QString::number(r)+" ==--\n");

    AddText2Views("======= Overall count of methods is "+QString::number(count_methods_all));

    auto printi{
        [this](const int v){
            static int counter = 0;
            counter++;
            this->AddText2Views(QString::number(counter)+" ("+typeid (v).name()+") "+QString::number(v));
        }
    };
    auto printd{
        [this](const double v){
            static int counter = 0;
            counter++;
            this->AddText2Views(QString::number(counter)+" ("+typeid (v).name()+") "+QString::number(v));
        }
    };
    auto prints{
        [this](const QString v){
            static int counter = 0;
            counter++;
            this->AddText2Views(QString::number(counter)+" ("+typeid (v).name()+") "+v);
        }
    };
    printi(11);
    printd(12.3);
    prints("first string (1)");
    prints("second string (2)");
    prints(QString("third string (3)"));
}

//void repeat(int repetitions, const std::function<void(int)>& fn)
//{
//    for (int i{ 1 }; i <= repetitions; i++){
//        fn(i);
//    }
//}

//lambda2
void MainWindow::on_pushButton_14_clicked()
{
    auto repeat{ [](int repetitions, const std::function<void(int)>& fn)
        {
            for (int i{ 1 }; i <= repetitions; i++){
                fn(i);
            }
        }
               };

    repeat(5, [this](int i){
        AddText2Views("try "+QString::number(i));
    });

    std::array months{ // если у вас не поддерживается C++17, то используйте std::array<std::string_view, 12>
      "January",
      "February",
      "March",
      "April",
      "May",
      "June",
      "July",
      "August",
      "September",
      "October",
      "November",
      "December"
    };

    // Поиск двух последовательных месяцев, которые начинаются с одинаковой буквы
    const char** sameLetter = std::adjacent_find(months.begin(), months.end(),
                                        [](const auto& a, const auto& b) {
                                          return (a[0] == b[0]);
                                        });

    // Убеждаемся, что эти два месяца были найдены
    if (sameLetter != months.end())
    {
      AddText2Views(QString::fromLocal8Bit(*sameLetter) +
                    " and " + QString::fromLocal8Bit(*std::next(sameLetter)) +
                    " start with the same letter");
    }
}

auto toloc(std::string_view s) -> QString
{
    return QString::fromLocal8Bit(s.data());
}

//string_view
void MainWindow::on_pushButton_15_clicked()
{
//    std::array<char> arr = "Test message for understand string_view";
    char arr[] = "Test message for understand string_view";
//    std::string_view s1 = "Test message for understand string_view";
    std::string_view s1 = arr;
    std::string_view s2 = s1;
    std::string_view s3 = s2;
    AddText2Views("String s1 is '"+toloc(s1)+"' ");
    AddText2Views("String s2 is '"+toloc(s2)+"' ");
    AddText2Views("String s3 is '"+toloc(s3)+"' ");
    AddText2Views("--change arr, s2--");
//    s1 = "Test (edit) message for understand string_view (add)";
    arr[10] = 'M';
    arr[11] = 'M';
    arr[12] = 'M';
    s2.remove_prefix(5);
//    std::string_view s4 = s3.substr(s3.find("view"), s3.length()-1);
//    std::string_view s5 = s3.substr(s3.find("view"), s3.length()-1)=="view" ? "Yes" : "No";
    AddText2Views("String s1 is '"+toloc(s1)+"' ");
    AddText2Views("String s2 is '"+toloc(s2)+"' ");
    AddText2Views("String s3 is '"+toloc(s3)+"' ");
    AddText2Views("Is s3 ends with 'view'? "+ QString::fromLocal8Bit(s3.substr(s3.find("view"), s3.length()-1)=="view" ? "Yes" : "No"));
}

struct Student{
    QString name;
    int mark;
};

//lambda_t1
void MainWindow::on_pushButton_16_clicked()
{
    std::array<Student, 8> arr{
      { { "Albert", 3 },
        { "Ben", 5 },
        { "Christine", 2 },
        { "Dan", 8 }, // Dan имеет больше всего баллов (8)
        { "Enchilada", 4 },
        { "Francis", 1 },
        { "Greg", 3 },
        { "Hagrid", 5 } }
    };
    AddText2Views("--==test 1==--");
    auto s = std::max_element(arr.begin(), arr.end(),
                              [](const Student& a, const Student& b) {return a.mark < b.mark;});
    AddText2Views("The best student (with the mark "+QString::number(s->mark)+") is "+s->name);
    AddText2Views("--==test 2==--");
    auto arr2 = arr;
    AddText2Views("before sort:");
    for(auto a:arr2){
        AddText2Views(a.name+" ("+QString::number(a.mark)+")");
    }
    std::sort(arr2.begin(), arr2.end(), [](const Student& a, const Student& b) {return a.mark < b.mark;});
    AddText2Views("after sort:");
    for(auto a:arr2){
        AddText2Views(a.name+" ("+QString::number(a.mark)+")");
    }
}

template <typename T> const T& mymax(const T& a, const T& b)
{
    return a>b ? a : b;
}

template <class T> T average(T* array, int length)
{
    T sum = 0;
    for(int i=0;i<length;sum+=array[i++]);
    return sum/length;
}

#include <sstream>
#include <QTextStream>

class Dollars{
public:
    Dollars(double val) : m_dollars(val){

    }
    friend bool operator>(const Dollars& a, const Dollars& b){
        return a.m_dollars>b.m_dollars;
    }
//this is equal to above operator> (in above, 'friend' is for
//declare non-class function(operator) and for access to private members)
//    bool operator>(const Dollars& b) const {
//        return m_dollars>b.m_dollars;
//    }
    operator QString(){
        QString s(QString::number(m_dollars));
        return s;
    }
//    explicit operator QString() const {
    operator QString() const {
        QString s(QString::number(m_dollars));
        return s;
    }
    Dollars& operator+= (const Dollars& b){
        m_dollars += b.m_dollars;
        return *this;
    }
    Dollars& operator/ (const int& b){
        m_dollars /= b;
        return *this;
    }
    friend QTextStream& operator<< (QTextStream &out, const Dollars &d){
        out << d.m_dollars << " dollars";
        return out;
    }
private:
    double m_dollars;
};


//template
void MainWindow::on_pushButton_17_clicked()
{
    auto print_max{ [this] (const auto& a, const auto& b){
            std::stringstream ss;
            ss << "From " << a << " and " << b << " max is " << mymax(a, b);
            AddText2Views(QString::fromStdString(ss.str()));
        }
                  };

    auto print_maxQ{ [this] (const auto& a, const auto& b){
            QString s;
            QTextStream ss(&s);
            ss << "From " << a << " and " << b << " max is " << mymax(a, b);
            AddText2Views(s);
        }
                  };

    print_max(2, 4);
    print_max(4.2, 4.0);
    print_max('a', 'b');
    print_max("string1", "string2");
    print_max("qwerty", "qWerty");

    print_maxQ(2, 4);
    print_maxQ(4.2, 4.0);
    print_maxQ('a', 'b');
    print_maxQ(QString("string"), QString("string2"));
    print_maxQ("qwerty3", "qwerty2");

    Dollars d1(8);
    Dollars d2(12);
//    print_maxQ(d1, d2);
    QString s = d1;
    const Dollars dm = mymax(d1, d2);
    AddText2Views("d1 is "+s+", d2 is "+d2+", max is "+static_cast<const QString>(dm));
    AddText2Views("d1 is "+d1+", d2 is "+d2+", max is "+static_cast<const QString>(mymax(d1, d2)));
    AddText2Views("d1 is "+s+", d2 is "+d2+", max is "+dm);
    AddText2Views("d1 is "+d1+", d2 is "+d2+", max is "+mymax(d1, d2));

    int arr1[] = {1,2,3,4,5};
    AddText2Views("arr1's average equal "+QString::number(average(arr1, 5)));
    double arr2[] = {1.5,2.2,3.5,4.1,5.2};
    AddText2Views("arr2's average equal "+QString::number(average(arr2, 5)));
    Dollars arr3[] = {Dollars(1), Dollars(2), Dollars(3), Dollars(4)};
    AddText2Views("arr3's average equal "+average(arr3, 4));

    print_maxQ(Dollars(6), Dollars(8));
}

template <class T> class ArrayNum{
private:
    T *m_data;
    size_t m_length;
public:
    ArrayNum(): m_data(nullptr), m_length(0) {

    };
    ArrayNum(size_t size){
        assert(size>0);
        m_length = size;
        m_data = new T[m_length];
    };
    ~ArrayNum(){
        delete[] m_data;
        m_data = nullptr;
        m_length = 0;
    };
    void Erase(){
        delete[] m_data;
        m_data = nullptr;
        m_length = 0;
    };
    size_t get_length(){
        return m_length;
    };
    T& operator[](size_t index){
        assert(index<m_length);
        return m_data[index];
    };
    QString toNum(size_t index){
        assert(index<m_length);
        return QString::number(m_data[index]);
    };
    void print(MainWindow *w);
//for test declar
    //{
//        w->AddText2Views("Array (size "+QString::number(get_length())+"):");
//        for (size_t i = 0; i < get_length(); i++){
//            w->AddText2Views("elem "+QString::number(i)+": "+toNum(i));
//        };
//    };
};

template <typename T>
void ArrayNum<T>::print(MainWindow* w){
    w->AddText2Views("Array (size "+QString::number(get_length())+"):");
    for (size_t i = 0; i < get_length(); i++){
        w->AddText2Views("elem "+QString::number(i)+": "+toNum(i));
    };
};

////////
/// ArraySta
///
template <typename T, size_t size>
class ArraySta
{
private:
    T m_data[size];
    size_t m_size = size;
    QString m_name;
public:
    ArraySta(std::string name) : m_name(QString::fromStdString(name)){
    }

    T& operator[](size_t index){
        assert(index<m_size);
        return m_data[index];
    };
    size_t get_length(){
        return m_size;
    };
    QString toNum(size_t index){
        assert(index<m_size);
        return QString::number(m_data[index]);
    };
    void print(std::function<void(QString)> printfunc);
//    void print(std::function<void(QString)> printfunc){
//        printfunc("Array "+m_name+"(size "+QString::number(get_length())+"):");
//        for (size_t i = 0; i < get_length(); i++){
//            printfunc("elem "+QString::number(i)+": "+toNum(i));
//        };
//    };
};

template <typename T, size_t size>
void ArraySta<T, size>::print(std::function<void(QString)> printfunc){
    printfunc("Array "+m_name+"(size "+QString::number(get_length())+"):");
    for (size_t i = 0; i < get_length(); i++){
        printfunc("elem "+QString::number(i)+": "+toNum(i));
    };
};

template<>
void ArraySta<double, 4>::print(std::function<void(QString)> printfunc){
    printfunc("Array "+m_name+" (size "+QString::number(get_length())+", type double):");
    for (size_t i = 0; i < get_length(); i++){
        printfunc("elem "+QString::number(i)+": "+toNum(i));
    };
};

#define DECLARE_ARRAYSTA(varname, type, size) \
    ArraySta <type, size> varname(#varname)

//template2
void MainWindow::on_pushButton_18_clicked()
{
    auto lprintfunc{ [this](QString s){ this->AddText2Views(s);} };
    ArrayNum <int> arr1(5);
    ArrayNum <double> arr2(4);
    for (size_t i = 0; i < arr1.get_length(); i++) arr1[i] = i+1;
    for (size_t i = 0; i < arr2.get_length(); i++) arr2[i] = (i+1)*1.1;
    arr1.print(this);
    arr2.print(this);

    DECLARE_ARRAYSTA(arr3, int, 5);
    ArraySta <double, 4> arr4("arr4");
    DECLARE_ARRAYSTA(arr5, double, 5);
    for (size_t i = 0; i < arr3.get_length(); i++) arr3[i] = (i+1)*3;
    for (size_t i = 0; i < arr4.get_length(); i++) arr4[i] = (i+1)*1.2;
    for (size_t i = 0; i < arr5.get_length(); i++) arr5[i] = (i+1)*2;
    arr3.print( lprintfunc );
    arr4.print( lprintfunc );
    arr5.print( this->AddText2ViewsS );
}








template <typename T>
class Repos
{
private:
    T* m_data = nullptr;
    size_t m_size = 0;
    QString m_name;
public:
    Repos(size_t size, std::string name) : m_size(size), m_name(QString::fromStdString(name)) {
        assert(size > 0);
        m_data = new T[m_size];
    };
    ~Repos(){
        delete [] m_data;
    };
    const QString& name(){
        return m_name;
    };
    const size_t& size(){
        return m_size;
    };
    void print(std::function<void(QString s)> printfunc){
        printfunc("(print from class) Array "+name()+" (size "+QString::number(size())+"):");
        for (size_t i=0; i<size(); i++) {
            printfunc(QString::number(i)+" : "+QString::number(m_data[i]));
        }
    };
    T operator[] (size_t index) const{
        assert(index<m_size);
        return m_data[index];
    };
    T& operator[] (size_t index){
        assert(index<m_size);
        return m_data[index];
    };
    operator double(){
        double sum = double();
        for(size_t i = 0; i<size(); i++){
            sum += m_data[i];
        }
        return sum/size();
    }
};

class ReposValueBool
{
private:
    uint8_t* m_value;
    uint8_t m_bit;
public:
    ReposValueBool(uint8_t* value, uint8_t bit) : m_value(value), m_bit(bit) {}
    operator bool() const {
        uint8_t mask = 0x01 << m_bit;
        return ((*m_value)&mask) !=0 ? true : false;
    }
    void operator= (const bool value) {
        uint8_t mask = 0x01 << m_bit;
        if(value){
            *m_value |= mask;
        }
        else{
            *m_value &= ~mask;
        }
    }
};

template <>
class Repos<bool>
{
private:
    uint8_t* m_data = nullptr;
    size_t m_size = 0;
    QString m_name;
public:
    Repos(size_t size, std::string name) : m_size(size), m_name(QString::fromStdString(name)) {
        assert(size > 0);
        m_data = new uint8_t[m_size/8+1];
        for (size_t i=0; i<this->size(); i++) {
            m_data[i] = 0;
        }
    };
    ~Repos(){
        delete [] m_data;
    };
    const QString& name(){
        return m_name;
    };
    const size_t& size(){
        return m_size;
    };
    void print(std::function<void(QString s)> printfunc){
        printfunc("(print from class) Array "+name()+" (size "+QString::number(size())+"):");
        for (size_t i=0; i<size(); i++) {
            uint8_t mask = 0x01 << i%8;
            printfunc(QString::number(i)+" : "+( (m_data[i/8]&mask) !=0 ?"true":"false"));
        }
    };
    ReposValueBool operator[] (size_t index){
        assert(index<m_size);
//        uint8_t mask = 0x01 << index%8;
//        return (m_data[index/8]&mask) !=0 ? true : false;
        return ReposValueBool(&m_data[index/8], index%8);
    };
    operator double(){
        double sum = double();
        for(size_t i = 0; i<size(); i++){
            sum += m_data[i];
        }
        return sum/size();
    }
};


#define DECLAREREPOS(name, type, size) Repos<type> name(size, #name)

//template3
void MainWindow::on_pushButton_19_clicked()
{
    auto print_repos{[this](auto& arr){
            this->AddText2Views("Array "+arr.name()+" (size "+QString::number(arr.size())+"):");
            for (size_t i=0; i<arr.size(); i++) {
                this->AddText2Views(QString::number(i)+" : "+QString::number(arr[i]));
            }
        }};

    DECLAREREPOS(a1, int, 5);
    for(size_t i=0; i<a1.size();i++) a1[i] = i+1;
    print_repos(a1);
    DECLAREREPOS(a2, double, 6);
    for(size_t i=0; i<a2.size();i++) a2[i] = (i+1)*1.1;
    print_repos(a2);
    a1.print(this->AddText2ViewsS);
    DECLAREREPOS(a3, bool, 9);
    for(size_t i=0; i<a3.size();i++) a3[i] = (i+1)%3;
    print_repos(a3);
    a3.print(this->AddText2ViewsS);
    for(size_t i=0; i<a3.size();i++) a3[i] = (i+1)%2;
    a3.print(this->AddText2ViewsS);
    AddText2Views("average for a1 is "+QString::number((double)a1));
    AddText2Views("average for a2 is "+QString::number((double)a2));
    AddText2Views("average for a3 is "+QString::number((double)a3));
}

class Rect
{
private:
    double m_x;
    double m_y;
public:
    Rect(double x, double y) : m_x(x), m_y(y) {};
    operator std::string() {
        std::string s = "rect("+std::to_string(m_x)+","+std::to_string(m_y)+")";
        return s;
    };
    operator double() {
        return m_x * m_y;
    };
};

//test double
void MainWindow::on_pushButton_20_clicked()
{
    Rect r(2.5, 3);
    AddText2Views("print rect sizes: "+QString::fromStdString(r));
    AddText2Views("print rect square: "+QString::number(r));
}

template <typename T1, typename T2>
class Pair
{
private:
    QString m_name;
    T1 first;
    T2 second;
public:
    Pair(QString const& name = "N/A", const T1& f = T1(), const T2& s = T2()) : m_name(name), first( f ), second( s ) {}
//    Pair(const Pair<T1, T2>& src) : m_name(QString("N/A")), first( src.first ), second( src.second ) {}
    Pair(const Pair<T1, T2>& src) : m_name(src.m_name), first( src.first ), second( src.second ) {}
    bool operator==( const Pair<T1, T2>& other) const {
        return first==other.first && second==other.second;
    }
    bool operator!=( const Pair<T1, T2>& other) const {
        return first!=other.first && second!=other.second;
    }
    bool operator< ( const Pair<T1, T2>& other ) const {
        return (first<other.first) || (first==other.first && second<other.second);
    }
    bool operator> ( const Pair<T1, T2>& other ) const {
        return (first>other.first) || (first==other.first && second>other.second);
    }
    bool operator<= ( const Pair<T1, T2>& other ) const {
        return (first<other.first) || (first==other.first && second<=other.second);
    }
    bool operator>= ( const Pair<T1, T2>& other ) const {
        return (first>other.first) || (first==other.first && second>=other.second);
    }
    Pair<T1, T2>& operator= ( const Pair<T1, T2>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }
    Pair<T1, T2> const& operator+= ( const Pair<T1, T2>& a) {
        this->first += a.first;
        this->second += a.second;
        return *this;
    }
    Pair<T1, T2> operator+ ( const Pair<T1, T2>& a) {
        Pair<T1, T2> n = *this;
        n += a;
        return n;
    }
    void swap(Pair<T1, T2>& s) {
        Pair<T1, T2> tmp("tmp");
        s = *this;
        *this = tmp;
    }
    void print( std::function<void(QString s)> func) {
        func("pair "+m_name+": "+QString::number(first)+", "+QString::number(second));
    }
};

#define Pair_Declare_3(name, t1, t2) \
    Pair <t1, t2> name(QString::fromLocal8Bit(#name))
#define Pair_Declare_4(name, t1, t2, first) \
    Pair <t1, t2> name(QString::fromLocal8Bit(#name), first)
#define Pair_Declare_5(name, t1, t2, first, second) \
    Pair <t1, t2> name(QString::fromLocal8Bit(#name), first, second)

#if 1
//it works! (1st version)
// The interim macro that simply strips the excess and ends up with the required macro
#define Pair_Declare_X(name, t1, t2, first, second, FUNC, ...)  FUNC
// The macro that the programmer uses
#define Pair_Declare(...)                 Pair_Declare_X(__VA_ARGS__, \
                                          Pair_Declare_5(__VA_ARGS__), \
                                          Pair_Declare_4(__VA_ARGS__), \
                                          Pair_Declare_3(__VA_ARGS__) \
                                         )
#else
//it works! (2st version)
#define GET_6TH_ARG(arg1, arg2, arg3, arg4, arg5, arg6, ...) arg6

#define Pair_Declare_CHOOSER(...) \
    GET_6TH_ARG( __VA_ARGS__, Pair_Declare_5, Pair_Declare_4, Pair_Declare_3, )

#define Pair_Declare(...) Pair_Declare_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#endif

#define Pair_DeclareCopy(name, t1, t2, src) \
    Pair <t1, t2> name(QString::fromLocal8Bit(#name), src)

//templ pair
void MainWindow::on_pushButton_21_clicked()
{
    Pair_Declare(p1, int, double);
    Pair_Declare(p2, int, double, 5);
    Pair_Declare(p3, int, double, 5, 7.1);
//    Pair_DeclareCopy(p4, int, double, p2);
    Pair_Declare(p4, int, double);
    p4 = p2;

    p1.print(this->AddText2ViewsS); //0, 0
    p2.print(this->AddText2ViewsS); //5, 0
    p3.print(this->AddText2ViewsS); //5, 7.1
    p4.print(this->AddText2ViewsS); //5, 0
    p2 += p3; //p2: 10, 7.1
    p1 = p2 + p3 + p4; //p1: 20, 14.2
    p3 = p4; //5, 0
    AddText2Views("(after math operations)");
    p1.print(this->AddText2ViewsS); //20, 14.2
    p2.print(this->AddText2ViewsS); //10, 7.1
    p3.print(this->AddText2ViewsS); //5, 0
    p4.print(this->AddText2ViewsS); //5, 0
    p1.swap(p3);
    p2.swap(p4);
    AddText2Views("(after swap operations)");
    p1.print(this->AddText2ViewsS); //5, 0
    p2.print(this->AddText2ViewsS); //5, 0
    p3.print(this->AddText2ViewsS); //20, 14.2
    p4.print(this->AddText2ViewsS); //10, 7.1
    Pair<int, double> p5("p5");
    p5.print(this->AddText2ViewsS);
}

template <typename T = int, size_t SIZE = 5>
class Array
{
    T m_data[SIZE];
    QString m_name;
public:
    class iterator;
    Array(const QString& name) : m_name(name) {
        for(size_t i=0; i<SIZE; i++){
            m_data[i] = T();
        }
    }
    T& operator[] (size_t index) {
        return m_data[index];
    }
    T operator[] (size_t index) const {
        assert(index<SIZE);
        return m_data[index];
    }
    template<typename T1>
    void CopyTo(T1& dest) {
        for(size_t i=0; i<SIZE; i++){
            dest[i] = static_cast<T>(m_data[i]);
        }
    }
    template<typename T1>
    void CopyFrom(const T1& src) {
        for(size_t i=0; i<SIZE; i++){
            m_data[i] = static_cast<T>(src[i]);
        }
    }
    void Fill(const std::initializer_list<T> &il) {
        size_t index = 0;
        for(auto i: il){
            m_data[index] = i;
            if(index++ >= SIZE) break;
        }
    }
    void print(std::function<void(QString)> printfunc) {
        printfunc("array " + m_name + " size is "+QString::number(SIZE));
        for(size_t i=0; i<SIZE; i++){
            printfunc(QString::number(i)+": "+QString::number(m_data[i]));
        }
    }
    class iterator{
    private:
        friend class Array;
        Array *m_arr;
        size_t m_index;
    public:
        explicit iterator(Array* arr) : m_arr(arr), m_index(0) {
            if(!arr){
                m_index = SIZE;
            }
        }
        T operator*() const {
            return (*m_arr)[m_index];
        }
        T* operator->() const {
            return &(*m_arr)[m_index];
        }
        iterator& operator++() {
            if(m_index<SIZE)
                m_index++;
            return *this;
        }
        bool operator==(iterator const& rhs) {
            return m_index==rhs.m_index;
        }
        bool operator!=(iterator const& rhs) {
            return m_index!=rhs.m_index;
        }
    };
    iterator begin() {
        return iterator(this);
    }
    iterator end() {
        return iterator(nullptr);
    }
};

//templ array
void MainWindow::on_pushButton_22_clicked()
{
    Array<> a1("a1");
    Array<int, 5> a2("a2");
    Array<double> a3("a3");
    Array<bool> a4("a4");
    Array<Pair<int, float>, 3> ap1("ap1");
    a1.Fill({1,2,3,4,5});
    a2.Fill({6,7,8,9,0});
    a3.Fill({2.2,3.3,4.4,5.5,6.6});
    a4.Fill({true,false,true,true,false});
    ap1.Fill({Pair<int, float>("e1", 2, 4.1), Pair<int, float>("e2", 3, 4.2), Pair<int, float>("e2", 4, 4.3)});
    AddText2Views("(print by iterator)");
    AddText2Views("--- a1");
    for(auto i : a1){
        AddText2Views(QString::number(i));
    }
    AddText2Views("--- a2");
    for(auto i : a2){
        AddText2Views(QString::number(i));
    }
    AddText2Views("--- a3");
    for(auto i : a3){
        AddText2Views(QString::number(i));
    }
    AddText2Views("--- a4");
    for(auto i : a4){
        AddText2Views(QString::number(i));
    }
    AddText2Views("--- a3");
    for(auto i = a3.begin(); i!=a3.end(); ++i){
        AddText2Views(QString::number(*i));
    }
    AddText2Views("--- ap1");
    for(auto i = ap1.begin(); i!=ap1.end(); ++i){
        i->print(AddText2ViewsS);
    }
    a1.print(AddText2ViewsS);
    a2.print(AddText2ViewsS);
    a3.print(AddText2ViewsS);
    a4.print(AddText2ViewsS);
    a1.CopyTo(a2);
    a1.CopyFrom(a3);
    AddText2Views("(after some copies - 1st experiments)");
    a1.print(AddText2ViewsS);
    a2.print(AddText2ViewsS);
    a3.print(AddText2ViewsS);
    a3.CopyTo(a1);
    a3.CopyFrom(a2);
    AddText2Views("(after some copies - 2nd experiments)");
    a1.print(AddText2ViewsS);
    a2.print(AddText2ViewsS);
    a3.print(AddText2ViewsS);
    AddText2Views("(after some copies - 3rd experiments)");
    std::array<float, 5> aa1 = {9.1,9.2,9.3,9.4,8.5};
    a1.CopyFrom(aa1);
    a3.CopyFrom(aa1);
    a1.print(AddText2ViewsS);
    a2.print(AddText2ViewsS);
    a3.print(AddText2ViewsS);
    AddText2Views("(after some copies - 4th experiments)");
    a2.CopyTo(aa1);
    a3.CopyFrom(aa1);
    a1.print(AddText2ViewsS);
    a2.print(AddText2ViewsS);
    a3.print(AddText2ViewsS);
}

class Currency
{
    uint32_t m_dollars;
    uint8_t m_cents;
public:
    Currency(uint32_t const& dollars = 0, uint8_t const& cents = 0)
        : m_dollars(dollars), m_cents(cents) {}
    Currency(const double& src)
        : m_dollars(static_cast<uint32_t>(src)), m_cents(static_cast<uint8_t>(fmod(src, 1)*100)) {}
//    operator double() {
//        return static_cast<double>(m_dollars)+static_cast<double>(m_cents)/100;
//    }
    operator double() const {
        return static_cast<double>(m_dollars)+static_cast<double>(m_cents)/100;
    }
    Currency& operator+= (const Currency& v) {
        m_dollars += v.m_dollars;
        m_cents += v.m_cents;
        m_dollars += m_cents/100;
        m_cents = m_cents%100;
        return *this;
    }
    void set(uint32_t const& dollars, uint8_t const& cents) {
        m_dollars = dollars;
        m_cents = cents;
    }
};

template<typename T>
T Add(T& n1, T& n2)
{
    return n1 + n2;
}

template< typename T >
double GetAverage(T tArray[], int nElements)
{
    T tSum = T(); // tSum = 0
    for (int nIndex = 0; nIndex < nElements; ++nIndex)
    {
        tSum += tArray[nIndex];
    }

    // Какой бы не был тип T, преобразовать его в double
    return double(tSum) / nElements;
}

template<typename T>
QString qstr(T const& v)
{
    return QString::number(v);
}

//templ cur
void MainWindow::on_pushButton_23_clicked()
{
    Currency c11(5, 18);
    AddText2Views("c1 is " + qstr<double>(c11) + " dollars");
    int i1 = 4, i2 = 5, i3 = Add(i1, i2);
    Currency c1(2,15), c2(4,50);
    Currency c3;
    c3 = Add(c1, c2);

    AddText2Views(qstr(i1)+" + "+qstr(i2)+" = "+qstr(i3));
    AddText2Views(qstr<double>(c1)+" dollars + "+qstr<double>(c2)+" dollars = "+qstr<double>(c3));

    Currency ar[] = {1.0, 3.50, 6.0, 6.0};
    AddText2Views("average is "+qstr(GetAverage(ar, 4)));

}

class Point
{
    int m_x, m_y;
public:
    Point(int x = 0, int y = 0) : m_x(x), m_y(y) {}
    Point(const Point& src) : m_x(src.m_x), m_y(src.m_y) {}
    operator double() {
        return static_cast<double>(m_x)+static_cast<double>(m_y)/100;
    }
    operator double() const {
        return static_cast<double>(m_x)+static_cast<double>(m_y)/100;
    }
    const QString GetStr() {
        return QString("<"+QString::number(m_x)+", "+QString::number(m_y)+">");
    }
    void operator= (const Point& src) {
        m_x = src.m_x*2;
        m_y = src.m_y*2;
    }
    void operator= (const int& src) {
        m_x = src+1;
        m_y = 0;
    }
    void operator= (const double& src) {
        m_x = src/1;
        m_y = (fmod(src,1)*100)/1;
    }
};

#define DISPLAYFUNC_PARAM 0

template <typename T>
class Item2
{
    QString m_name;
    T m_data;
public:
    Item2(const QString& name = "N/A", const T& data = T()) : m_name(name), m_data(data) {}
    void SetData(const T& src) {
        m_data = src;
    }
    const T& GetData() const {
        return m_data;
    }
    template<typename TDisplayFunc>
#if DISPLAYFUNC_PARAM
    void print(TDisplayFunc printfunc) {
#else
    void print() {
    TDisplayFunc printfunc;
#endif
        printfunc(m_name+": "+QString::number(m_data));
    }
    template<typename U>
    void operator= (const U& src) {
        m_data = src.GetData();
    }
    void operator= (const double& src) {
        m_data = src;
    }
};

#if 0
template<>
template<>
void Item2<Point>::print(std::function<void(QString)> printfunc ) {
    printfunc(m_name+": "+QString::number(m_data, 'f', 2));
}
#else
template<>
template<typename TDisplayFunc>
#if DISPLAYFUNC_PARAM
void Item2<Point>::print(TDisplayFunc printfunc) {
#else
void Item2<Point>::print() {
    TDisplayFunc printfunc;
#endif
    printfunc(m_name+": "+m_data.GetStr());
}
#endif

struct DisplayHelper
{
    void operator() (const QString& s) {
        MainWindow::AddText2ViewsS(s);
    }
};

template<typename T, typename TDisplayFunc>
void _printfunc(T& src, TDisplayFunc printfunc) {
    printfunc("(from _printfunc) "+QString::number(src.GetData()));
}

template<typename TDisplayFunc, typename T>
void _printfunc2(T& src) {
    TDisplayFunc printfunc;
    printfunc("(from _printfunc2) "+QString::number(src.GetData()));
}

//templ point
void MainWindow::on_pushButton_24_clicked()
{
#if 1
    auto printlambda = [this] (const QString& s) {
            this->AddText2Views("(from lambda) "+s);
        };
//    auto printlambda = [] (QString s) {
//            MainWindow::AddText2ViewsS("(from lambda) "+s);
//        };
//    std::function<void(const QString&)> printlambda = [this] (const QString& s) {
//        this->AddText2Views("(from lambda) "+s);
//    };
    Item2<int> item1("item1");
    Item2<int> item2("item2", 5);
    Item2<Point> item3("item3", Point(5,8));
    Item2<Point> item4("item4");
    double d = 9.89;
    item4 = d;
    Item2<double> item5("item5", 3.56);
#if DISPLAYFUNC_PARAM
    item1.print(AddText2ViewsS);
    item2.print(AddText2ViewsS);
    item3.print(AddText2ViewsS);
    item4.print(printlambda);
    item5.print(printlambda);
    DisplayHelper dh;
    AddText2Views("(print with DisplayHelper)");
    item1.print(dh);
    item2.print(dh);
    item3.print(dh);
    item4.print(dh);
    item5.print(dh);
#else
    item1.print<DisplayHelper>();
    item2.print<DisplayHelper>();
    item3.print<DisplayHelper>();
//    _printfunc<DisplayHelper>(item4);
//    _printfunc<DisplayHelper>(item5);
//    _printfunc<decltype(printlambda)>(item4);

    _printfunc(item4, printlambda);
    _printfunc(item5, printlambda);
    DisplayHelper dh;
    _printfunc(item4, dh);
    _printfunc(item5, dh);

    _printfunc2<DisplayHelper>(item4);
    _printfunc2<DisplayHelper>(item5);
//TODO: in example this work, but in my program nope
//    _printfunc2<decltype(printlambda)>(item4);

//    _printfunc<decltype(printlambda)>(item5);
//    item4.print<decltype(printlambda)>();
//    item5.print<decltype(printlambda)>();
#endif

#else
    Item2<int> item1("item1");
    Item2<int> item2("item2", 5);
    Item2<Point> item3("item3", Point(5,8));
    Item2<Point> item4("item4");
    Item2<double> item5("item5", 3.56);
    AddText2Views("(after some conversions)");
    item4.SetData(item3.GetData());
    item3.print(AddText2ViewsS);
    item4.print(AddText2ViewsS);
    AddText2Views("(after some conversions 2)");
    item3.SetData(Point(4, 6));
    item3.print(AddText2ViewsS);
    item4.print(AddText2ViewsS);
    AddText2Views("(after some conversions 3)");
    item4 = item3;
    item3.print(AddText2ViewsS);
    item4.print(AddText2ViewsS);
    AddText2Views("(after some conversions 4)");
//    item4 = item5;
    item4.print(AddText2ViewsS);
    AddText2Views("(after some conversions 5)");
    item4 = item2;
    item4.print(AddText2ViewsS);
    AddText2Views("(-------------------- Set AverageFrom --------------)");
#endif
}

template <typename T>
class smart_ptr
{
    T* m_ptr;
public:
    smart_ptr(T* p = nullptr) : m_ptr (p) {}
    ~smart_ptr() {
        if(m_ptr)
            delete m_ptr;
    }
    T& operator*() {
        return *m_ptr;
    }
    T operator*() const {
        return *m_ptr;
    }
    T* operator-> () {
        return m_ptr;
    }
    const T* operator-> () const {
        return m_ptr;
    }
};

template<typename T>
void show_ptr_val(const smart_ptr<T>& val, std::function<void(QString)> printfunc)
{
    printfunc(QString::number(*val));
}

//templ ptr
void MainWindow::on_pushButton_25_clicked()
{
    smart_ptr<int> p1(new int);
    smart_ptr<Currency> p2(new Currency(10, 45));
    *p1 = 10;
    show_ptr_val(p1, AddText2ViewsS);
    show_ptr_val(p2, AddText2ViewsS);
    p2->set(10, 23);
    show_ptr_val(p2, AddText2ViewsS);
}

template <typename TDerived>
class WorkerCoreT
{
public:
    WorkerCoreT() {}
    template<typename TPrintFunc>
    void ProcessNumbers(int nStart, int nEnd) {
        TPrintFunc printfunc;
        for(; nStart <= nEnd; nStart++){
            TDerived& tDerivedObj = (TDerived&)*this;
            tDerivedObj.ProcessOne(nStart, printfunc);
        }
    }
};

class ActualWorkerT : public WorkerCoreT<ActualWorkerT>
{
public:
    ActualWorkerT() {}
    template<typename TPrintFunc>
    void ProcessOne(int nNumber, TPrintFunc printfunc) {
        printfunc("process "+QString::number(nNumber));
    }
};

//templ inherit
void MainWindow::on_pushButton_26_clicked()
{
    ActualWorkerT* pWorker = new ActualWorkerT();
    pWorker->ProcessNumbers<DisplayHelper>(5, 12);
}

template<typename TDisplayFunc, typename T>
void printvector(const std::vector<T>& vec) {
    TDisplayFunc printfunc;
    printfunc("vector size is "+QString::number(vec.size())+", capacity is "+QString::number(vec.capacity()));
    size_t currIndex = 0;
    for(auto i : vec){
        printfunc(QString::number(currIndex++)+": "+QString::number(i));
    }
}

#include <set>

template<typename TDisplayFunc, typename T>
void printcollection(const T& v)
{
    TDisplayFunc printfunc;
    printfunc("collection size is "+QString::number(v.size())+", capacity is "+QString::number(v.capacity()));
    size_t currIndex = 0;
    for(auto i : v){
        printfunc(QString::number(currIndex++)+": "+QString::number(i));
    }
}

//STL 1
void MainWindow::on_pushButton_27_clicked()
{
    auto printvector_l1 = [this](const std::vector<std::pair<int, Currency>>& vec) {
        AddText2Views("vector size is "+QString::number(vec.size())+", capacity is "+QString::number(vec.capacity()));
        for(auto i : vec){
            AddText2Views(QString::number(i.first)+": "+QString::number(i.second));
        }
    };

    std::vector<int> IntVector = {2,4,6,8};
    IntVector.push_back(10);
    int counter = 0;
    for_each(IntVector.begin(), IntVector.end(), [this, &counter] (const auto& i) {
        this->AddText2Views(QString::number(++counter)+": "+QString::number(i));
    });
    IntVector.pop_back();
    IntVector.pop_back();
    IntVector.pop_back();
    foreach(auto i, IntVector) {
        this->AddText2Views(QString::number(++counter)+": "+QString::number(i));
    }

    printvector<DisplayHelper>(IntVector);

    std::vector<Currency> CurVector;
    CurVector.push_back(Currency(2,70));
    CurVector.push_back(Currency(3,71));
    CurVector.push_back(Currency(4,72));
    CurVector.push_back(Currency(5,73));
    CurVector.push_back(Currency(6,74));
    printvector<DisplayHelper>(CurVector);

    auto printpair_l = [this] <typename TF, typename TS> (const std::pair<TF, TS>& v) {
        this->AddText2Views("(templ lambda) "+QString::number(v.first)+": "+QString::number(v.second));
    };
    std::vector<std::pair<int, Currency>> Cur2Vector;
    Cur2Vector.push_back(std::pair<int, Currency>(1, Currency(3, 56)));
    Cur2Vector.push_back(std::pair<int, Currency>(2, Currency(4, 57)));
    Cur2Vector.push_back(std::pair<int, Currency>(3, Currency(5, 58)));
    Cur2Vector.push_back(std::pair<int, Currency>(4, Currency(6, 59)));
    Cur2Vector.push_back(std::pair<int, Currency>(5, Currency(7, 60)));
    Cur2Vector.push_back(std::pair<int, Currency>(6, Currency(8, 61)));
    printvector_l1(Cur2Vector);
    AddText2Views("(templ lambda) vector size is "+QString::number(Cur2Vector.size())+", capacity is "+QString::number(Cur2Vector.capacity()));
    std::for_each(Cur2Vector.begin(), Cur2Vector.end(), printpair_l);

    AddText2Views("(set)");
    std::set<int> IntSet;
    IntSet.insert(25);
    IntSet.insert(54);
    IntSet.insert(14);
    IntSet.insert(70);
    IntSet.insert(5);
    int set_counter = 0;
    std::for_each(IntSet.begin(), IntSet.end(), [this, &set_counter] (const auto& i) {
        this->AddText2Views(QString::number(++set_counter)+": "+QString::number(i));
    });
    set_counter = 0;
    for(auto i : IntSet) {
        this->AddText2Views(QString::number(++set_counter)+": "+QString::number(i));
    }
    auto *pCur3Vector = new std::vector<Currency>;
    pCur3Vector->push_back(Currency(12,23));
    pCur3Vector->push_back(Currency(11,34));
    pCur3Vector->push_back(Currency(10,45));
    pCur3Vector->push_back(Currency(9,56));
    printvector<DisplayHelper>(*pCur3Vector);

    printcollection<DisplayHelper>(*pCur3Vector);
}
















