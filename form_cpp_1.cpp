#include "form_cpp_1.h"
#include "ui_form_cpp_1.h"
#include <ctime>

namespace myconsts {
constexpr int32_t c_timerStepMs = 100;
constexpr double c_EarthG = 9.8;
const double c_inf = 1.0/0;
}

struct outputHelper{
public:
    void operator() (const QString& str) {
        Form_cpp_1::outputS(str);
    };
};

Form_cpp_1* Form_cpp_1::form_cpp_1_ptr = nullptr;

Form_cpp_1::Form_cpp_1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_cpp_1)
{
    ui->setupUi(this);
    form_cpp_1_ptr = this;
    m_currentObjectHeight = 0.0;
    m_currentObjectSpeed = 0.0;
    connect(&m_timerObjectHeight, SIGNAL(timeout()), this, SLOT(checkObjectHeight()));
}

Form_cpp_1::~Form_cpp_1()
{
    form_cpp_1_ptr = nullptr;
    delete ui;
}

Form_cpp_1& Form_cpp_1::getForm_cpp_1_ptr()
{
    return *Form_cpp_1::form_cpp_1_ptr;
}

void Form_cpp_1::output(const QString &str)
{
    ui->textEdit->append(str);
}

template<typename TNumber>
const QString qnum(const TNumber& n)
{
    return QString::number(n);
}

void Form_cpp_1::outputTime()
{
    std::time_t t = std::time(0);
    struct std::tm* tmt = std::localtime(&t);
    output("");
    output("---=== "+QString("%1:%2:%3")
           .arg(tmt->tm_hour, 2, 10, QLatin1Char('0'))
           .arg(tmt->tm_min, 2, 10, QLatin1Char('0'))
           .arg(tmt->tm_sec, 2, 10, QLatin1Char('0'))
           +" ===---"
           );
}

void Form_cpp_1::outputS(const QString &str)
{
    getForm_cpp_1_ptr().output(str);
}

void Form_cpp_1::outputTimeS()
{
    Form_cpp_1::getForm_cpp_1_ptr().outputTime();
}

//Close
void Form_cpp_1::on_pushButton_clicked()
{
    exit(0);
}

//Clear Output
void Form_cpp_1::on_pushButton_4_clicked()
{
    ui->textEdit->clear();
}

template<typename T, int SIZE>
class TestClass01
{
    T m_data[SIZE];
public:
    TestClass01() { }
    void Fill(const std::initializer_list<T>& il) {
        size_t counter = 0;
        for(auto n : il){
            m_data[counter] = n;
            if(++counter >= SIZE) return;
        }
    }
    template<typename TPrintFunc>
    void print() {
        TPrintFunc printfunc;
        for(size_t i=0; i<SIZE; ++i){
            printfunc(qnum(i)+": "+qnum(m_data[i]));
        }
    }
};

//test01
void Form_cpp_1::on_pushButton_3_clicked()
{
    outputTime();
    output("test msg 01");
    output("test msg 02");
    TestClass01<int, 8> arr1;
    arr1.Fill( {1,3,5,7,9,11,13,17,19} );
    arr1.print<outputHelper>();
}

#include <cmath>
#include <set>

namespace mytypes {
enum Operators {
    Add = 0,
    Odd = 1,
    Div = 2,
    Mult = 3
};
}

//lesson30-...
void Form_cpp_1::on_pushButton_2_clicked()
{
    ////////////////
    // lesson 30-33
    outputTime();
    output("---=== lesson 30-33");
    output("size of int         - "+qnum(sizeof(int)));
    output("size of long        - "+qnum(sizeof(long)));
    output("size of long long   - "+qnum(sizeof(long long)));
    output("size of double      - "+qnum(sizeof(double)));
    output("size of long double - "+qnum(sizeof(long double)));
    output("size of wchar_t     - "+qnum(sizeof(wchar_t)));
    int32_t i{};
    output("size of i           - "+qnum(sizeof(i)));
    double d = 1.234e0;
    output("d is " + qnum(d));
    float f{123456789.0};
    output("f is " + qnum(static_cast<int>(f)));
    double dzero = 0.0;
    double inf = d/dzero;
    output("inf is "+qnum(inf));
    d = -5.0;
    inf = d/dzero;
    output("inf is "+qnum(inf));
    inf = dzero/dzero;
    output("inf is "+qnum(inf));
    ////////////////
    // lesson 34
    output("---=== lesson 34");
    std::set<int> sampleDigits{2,3,5,7,11,13,17,19};
    for (int digit=0; digit<20; digit++ ) {

        if( sampleDigits.find(digit)!=sampleDigits.end() ){
            output(qnum(digit)+" is Simple Digit");
        }
        else{
            output(qnum(digit)+" is NOT Simple Digit");
        }
    }
    ////////////////
    // lesson 36
    output("---=== lesson 36");
    uint32_t bigNum = 2'345'678'900;
    output("i is "+qnum(bigNum));
    ////////////////
    // test after lesson 36 (2-3)
    double da = ui->lineEdit_numA->text().toDouble();
    double db = ui->lineEdit_numB->text().toDouble();
    double result{0};
    QString textOperator{};
    switch (ui->comboBox_operator->currentIndex()){
    case mytypes::Operators::Add :
        result = da+db;
        textOperator = " + ";
        break;
    case mytypes::Operators::Odd :
        result = da-db;
        textOperator = " - ";
        break;
    case mytypes::Operators::Div :
        result = da/db;
        textOperator = " / ";
        break;
    case mytypes::Operators::Mult :
        result = da*db;
        textOperator = " * ";
        break;
    }
    output(qnum(da)+textOperator+qnum(db)+" = "+qnum(result));
    ui->label_result->setText(" = "+qnum(result));

    ////////////////
    // test after lesson 36 (2-4)
    m_currentObjectHeight = ui->lineEdit_hFromEarth->text().toDouble();
    m_currentObjectSpeed = 0.0;
    if(m_currentObjectHeight > 0){
        ui->label_fallObjectResult->setText(" h = "+QString::number(m_currentObjectHeight, 'f', 2)+" m, "+
                                            "V = "+QString::number(m_currentObjectSpeed, 'f', 2)+" m/s");
        m_timerObjectHeight.start(myconsts::c_timerStepMs);
    }
}

// Возвращаем true, если разница между a и b в пределах процента эпсилона
bool approximatelyEqual(double a, double b, double epsilon)
{
    return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(a) : fabs(b)) * epsilon);
}

// Возвращаем true, если разница между a и b меньше absEpsilon или в пределах relEpsilon
bool approximatelyEqualAbsRel(double a, double b, double absEpsilon = 1e-12, double relEpsilon = 1e-8)
{
    // Проверяем числа на равенство их друг другу - это нужно в случаях, когда сравниваемые числа являются нулевыми или около нуля
    double diff = fabs(a - b);
    if (diff <= absEpsilon)
        return true;

    // В противном случае, возвращаемся к алгоритму Кнута
    return diff <= ((fabs(a) > fabs(b) ? fabs(a) : fabs(b)) * relEpsilon);
}

void Form_cpp_1::checkObjectHeight()
{
    using myconsts::c_timerStepMs;
    using myconsts::c_EarthG;
    double timerStepSec = c_timerStepMs/1000.0;
    m_currentObjectHeight -= m_currentObjectSpeed*timerStepSec + c_EarthG*pow(timerStepSec, 2)/2;
    m_currentObjectSpeed += c_EarthG*timerStepSec;
    if(m_currentObjectHeight<0){
        m_currentObjectHeight = 0.0;
        m_timerObjectHeight.stop();
    }

    ui->label_fallObjectResult->setText(" h = "+QString::number(m_currentObjectHeight, 'f', 2)+" m, "+
                                        "V = "+QString::number(m_currentObjectSpeed, 'f', 2)+" m/s");
    int currentCountSec = (m_currentObjectSpeed+0.001) / c_EarthG;
//    if( abs(static_cast<int>(m_currentObjectSpeed*1000.0) - static_cast<int>(myconsts::c_EarthG*1000.0)*countSec) < 10 ){
    if( approximatelyEqualAbsRel(m_currentObjectSpeed, c_EarthG*currentCountSec, 1e-12, 1e-8)){
        output(qnum(currentCountSec)+" s: "+" h = "+QString::number(m_currentObjectHeight, 'f', 2)+" m, "+
               "V = "+QString::number(m_currentObjectSpeed, 'f', 2)+" m/s");
    }
}

int pow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

#include <bitset>

//lesson38-...
void Form_cpp_1::on_pushButton_5_clicked()
{
    output("2^9 = "+qnum(pow(2, 9)));
    int a{23};
    int b = -(--a);
    output("a = "+qnum(a)+"   b = "+qnum(b));
    double d = 10/0.03;
    if(d == myconsts::c_inf){
        output("d is inf (d = "+qnum(d)+")");
    }
    else{
        output("d = "+qnum(d));
    }
    output("-5 % 2 = "+qnum(-5%2)+", 5%-2 = "+qnum(5%-2));

    double d1 = 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1;
    output(qnum(approximatelyEqual(d1, 1.0, 1e-8)));
    output(qnum(approximatelyEqual(d1-1.0, 0.0, 1e-8)));
    output(qnum(approximatelyEqualAbsRel(d1-1.0, 0.0, 1e-12, 1e-8)));

    ////////////////////
    output("---=== lesson 44 ===---");
    for(u_int8_t i = 0; i<UINT8_MAX; ++i){
        output("uint: "+qnum(i)+", int: "+qnum(static_cast<int8_t>(i)));
    }

    ////////////////////
    output("---=== lesson 46 ===---");
    std::bitset<16> bs(0x04 | 0x80);
    output(QString::fromStdString(bs.to_string()));
    if( bs.test(0) ){
        output("bit 0 is true");
    }
    bs <<= 2;
    bs |= 0x8001;
    output(QString::fromStdString(bs.to_string()));
    if( bs.test(0) ){
        output("bit 0 is true");
    }
    bs.flip();
    output(QString::fromStdString(bs.to_string()));
    if( bs.test(0) ){
        output("bit 0 is true");
    }
    output("bit size is "+qnum(bs.size())+", size in memory is "+qnum(sizeof(bs)));
    constexpr unsigned char option_viewed = 0x01;
//    constexpr unsigned char option_edited = 0x02;
    constexpr unsigned char option_favorited = 0x04;
//    constexpr unsigned char option_shared = 0x08;
    constexpr unsigned char option_deleted = 0x80;

    unsigned char myArticleFlags = 0;
    myArticleFlags |= option_viewed;
    if( myArticleFlags&option_deleted ){
        output("deleted");
    }
    myArticleFlags &= ~option_favorited;
    output(qnum(7/-2)+" "+qnum(5%-2));

}

extern int g_var1;
extern double g_var2;

#include <typeinfo>

/////////////////////////////
// namespace myenums
namespace myenums {

enum Colors{
    COLOR_BLUE,
    COLOR_RED,
    COLOR_BROWN,
    COLOR_GRAY,
    COLOR_WHITE
};

enum ColorsBW{
    COLOR_BW1,
    COLOR_BW2,
    COLOR_BW3,
    COLOR_BW4,
};

enum class Warriors{
    WARRIOR_OGRE,
    WARRIOR_GOBLIN,
    WARRIOR_SKELETON,
    WARRIOR_ORC,
    WARRIOR_TROLL,
};

QString warrior_type(const Warriors& w){
    switch (w) {
    case Warriors::WARRIOR_OGRE:
        return QString("ogre");
    case Warriors::WARRIOR_GOBLIN:
        return QString("goblin");
    case Warriors::WARRIOR_SKELETON:
        return QString("skeleton");
    case Warriors::WARRIOR_ORC:
        return QString("orc");
    case Warriors::WARRIOR_TROLL:
        return QString("troll");
    default:
        return QString("-");
    }
}

QString warrior_type2(const Warriors& w){
    switch (w) {
    case Warriors::WARRIOR_OGRE:
        return QString("ogre");
    case Warriors::WARRIOR_GOBLIN:
        return QString("goblin");
    case Warriors::WARRIOR_SKELETON:
        return QString("skeleton");
    case Warriors::WARRIOR_ORC:
        return QString("orc");
    case Warriors::WARRIOR_TROLL:
        return QString("troll");
    default:
        return QString("-");
    }
}

}
// namespace myenums
/////////////////////////////

/////////////////////////////
// namespace myfuncs
namespace myfuncs {

typedef int TMyInt;

enum TestEnum {
    One = 1,
    Two = 2
};

template<typename TPrintFunc>
void testfunc1(const TMyInt i){
    TPrintFunc printfunc;
    printfunc("Some print from testfunc1 (arg is "+qnum(i)+")");
}

void testfunc2(const TestEnum& te){
    Form_cpp_1::outputS("from testfunc2: "+qnum(te));
}

void testfunc3(const TMyInt i){
    Form_cpp_1::outputS("from testfunc3: "+qnum(i));
}

}
// namespace myfuncs
/////////////////////////////

template<typename T>
std::vector<T> returnSimpleSet(T value, bool isFirstStart = true)
{
    static std::vector<T> returnVector;
    if(isFirstStart){
        returnVector.clear();
    }
    for(T i = 2; i<=value; ++i){
        if(value%i == 0){
            returnVector.push_back(i);
            value /= i;
            returnSimpleSet(value, false);
            break;
        }
    }
    return returnVector;
}

//lesson 49-...
void Form_cpp_1::on_pushButton_6_clicked()
{
    output("---=== lesson 49 ===---");
    output("g_var1 = "+qnum(g_var1)+", g_var2 = "+qnum(g_var2));
    output("---=== lesson 51 ===---");
    static uint8_t s_on_pushButton_6_clicked_counter{0};
    output("count click pushButton6 is "+qnum(++s_on_pushButton_6_clicked_counter));
    output("---=== lesson 54 ===---");
    using std::string;
    string s = "Hello by std::string";
    output(QString::fromStdString(s));
    using namespace std;
    vector<int> v{5, 13, 18, 7, 119};
    for_each(v.begin(), v.end(), [] (auto it){
        outputS(qnum(it));
    });
    output("---=== lesson 55 ===---");
    double d = 1.23456789123456789123456789;
    float f = d;
    output("double - "+QString::number(d, 'f', 30)+", float - "+QString::number(f, 'f', 20));
    output("type of d+f is "+QString::fromLocal8Bit(typeid(d+f).name()));
    output("type of 7+5.1 is "+QString::fromLocal8Bit(typeid(7+5.1).name()));
    output("type of 7+5 is "+QString::fromLocal8Bit(typeid(7+5).name()));
    output("hash code of i is "+qnum(typeid(12).hash_code()));
    output("hash code of d is "+qnum(typeid(12.1).hash_code()));
    output("hash code of f is "+qnum(typeid(12.1f).hash_code()));
    output("hash code of u is "+qnum(typeid(8u).hash_code()));
    int i = -5;
    output("i is "+qnum(i));
    i <<=1;
    output("i is "+qnum(i));
    output("---=== lesson 56 ===---");
//    const int c_i{6};
    uint8_t i8 = static_cast<uint8_t>(i);
    output(qnum(i8));
    output("---=== lesson 57 ===---");
    std::string ss{"Hello мій дружеs!"};
    output("string: "+QString::fromStdString(ss));
    std::for_each(ss.begin(), ss.end(), [this](auto c) {
        output(QChar::fromLatin1(c));
    });
    QString qs = QString::fromStdString(ss);
    output("qs - "+ qs);
    std::for_each(qs.begin(), qs.end(), [this] (auto c) {
        output(c);
    });
    output("---=== lesson 58 ===---");
    myenums::Colors mc{myenums::COLOR_RED};
    output("enum mc is "+qnum(mc)+", type name is "+QString::fromLocal8Bit(typeid(mc).name())+", sizeof is "+qnum(sizeof(mc)));
    myenums::Warriors wr{myenums::Warriors::WARRIOR_OGRE};
    output("We have "+warrior_type(wr)+" warrior "+warrior_type(wr));
    wr = static_cast<myenums::Warriors>(45);
    output("We have "+myenums::warrior_type(wr)+" warrior and "+warrior_type(myenums::Warriors(myenums::Warriors::WARRIOR_SKELETON)));
    myenums::Colors c1{myenums::COLOR_RED};
    myenums::Warriors w1{myenums::Warriors::WARRIOR_GOBLIN};
    if( static_cast<int>(c1) == static_cast<int>(w1) ){
        output("c1 and w1 is equal");
    }
    warrior_type2(myenums::Warriors(0));

    myfuncs::testfunc1<outputHelper>(myfuncs::TMyInt(5));
    testfunc2(myfuncs::TestEnum::Two);
    testfunc2( static_cast<myfuncs::TestEnum>(5));
//    myfuncs::TMyInt ii = 4;
    myfuncs::testfunc3( static_cast<myfuncs::TMyInt>(myfuncs::TestEnum::One));
//    myenums::Colors c2 = myenums::COLOR_BLUE;
//    myenums::ColorsBW c3 = myenums::COLOR_BW4;
//    myenums::Warriors c4 = myenums::Warriors::WARRIOR_SKELETON;
//    if( c2 == c3 ){ //WARNING compare
//        output("c2 equal c3");
//    }
//    if( c2 == c4 ){ //ERROR compare
//        output("c2 equal c3");
//    }
    output("---=== lesson 61 ===---");
    using vectorInt_t = std::vector<int>;
    int i1 = ui->lineEdit_lesson49->text().toInt();
    int i2 = ui->lineEdit_lesson49_2->text().toInt();
    vectorInt_t vec1 = returnSimpleSet<int>(i1);
    vectorInt_t vec2 = returnSimpleSet<int>(i2);
    auto printSet { [this] (const int& value, const vectorInt_t& vec) {
            QString sSimpleSet = qnum(value) + ": ";
            for(auto i : vec){
                sSimpleSet += qnum(i)+" ";
            }
            output(sSimpleSet);
        }};
    printSet(i1, vec1);
    printSet(i2, vec2);
    auto reduceFraction { [] (int& value1, int& value2, const vectorInt_t& vec) {
            std::for_each(vec.begin(), vec.end(), [&value1, &value2](auto i) {
                if( value1%i==0 && value2%i==0 ){
                    value1 /= i;
                    value2 /= i;
                }
            });
        }};
    reduceFraction(i1, i2, vec1);
    reduceFraction(i1, i2, vec2);
    ui->label_lesson49->setText("= "+qnum(i1)+" / "+qnum(i2));
}

//test02
void Form_cpp_1::on_pushButton_7_clicked()
{

}


//lesson 63-...
void Form_cpp_1::on_pushButton_8_clicked()
{
//    output("---=== lesson 65 ===---");
//    switch (2) {
//    case 2:
//        int v = 4; //ERROR: can't initialize variable (good is: int v; v = 4; OR case 2: {int v = 4;...} )
//        output(qnum(v));
//        break;
//    case 3:
//        output("3");
//        break;
//    }
#if 0
    output("---=== lesson 67 ===---");
    int outer{0};
    while (++outer <= 5) {
        QString s{};
        int inner{6};
        while (--inner >= 1) {
            s += (inner>outer) ? "   " : qnum(inner)+" ";
        }
        output(s);
    }
    output("---=== lesson 69 ===---");
    for (int outer=1; outer<=5; ++outer) {
        QString s{};
        for (int inner=5; inner>=1; --inner) {
            s += (inner>outer) ? "   " : qnum(inner)+" ";
        }
        output(s);
    }
    for (int outer=1; outer<=5; ++outer) {
        QString s{};
        for (int inner=5; inner>=1; --inner) {
            s += (inner>outer) ? "   " : qnum(inner)+" ";
        }
        output(s);
    }
    int i{0}; float b{0.0};
    for(i = 0, b = 5.0; i < 5; ++i, --b){
        output(qnum(i)+" "+qnum(b));
    }
#endif
#if 0
    output("---=== lesson 69 ===---");
    static bool isFirstStartForRand{true};
    if(isFirstStartForRand){
        srand(time(nullptr));
        isFirstStartForRand = false;
    }
    auto myrandr{ [] (int min, int max) -> int {
            return min+rand()%(max-min+1);
        }};
    output(qnum(myrandr(11,16)));

    //first rand
    {
    const int countIteration{10000000};
    const int minR{3};
    const int maxR{7};
    assert(minR<maxR);
    std::array<std::pair<int, int>, maxR-minR+1> countRand{};
    for(size_t i=0; i<countRand.size(); ++i){
        countRand[i].first = i+minR;
    }

    for(int i = 0; i<countIteration; ++i){
        countRand[myrandr(minR,maxR)-minR].second++;
    }
    for(size_t i=0; i<countRand.size(); ++i){
        output("Number "+qnum(countRand[i].first)+" rand "+qnum(countRand[i].second)+" times ("+
               QString::number(static_cast<double>(countRand[i].second)/countIteration*100, 'f', 2)+"%)");
    }
    }

    //second rand
    {
    const int c_countIteration{10000000};
    const int c_countFoundDublication{1000};
    std::set<int> unicNumbers{};
    for(int i = 0, dublicateCounter = 0; i<c_countIteration && dublicateCounter<c_countFoundDublication; ++i){
        int r = rand();
        if(unicNumbers.find(r)!=unicNumbers.end()){
            dublicateCounter++;
            if(dublicateCounter==1)
                output("Dublicate rand on "+qnum(i)+" iteration (1st dublication)");
            if(dublicateCounter==c_countFoundDublication)
                output("Dublicate rand on "+qnum(i)+" iteration ("+qnum(dublicateCounter)+"th dublication");
        }
        else{
            unicNumbers.insert(r);
        }
    }
    }
#endif
#if 0
    output("---=== test 01 ===---");
    using myconsts::c_EarthG;
    const double c_initialHeight{5000.0};
    double timeToEarth = sqrt(c_initialHeight*2/c_EarthG);
    auto calculateHeight_TrueOnEarth{ [this, c_initialHeight, timeToEarth] (double seconds) -> bool {
            bool isOnEarth = false;
            if(seconds>timeToEarth){
                seconds = timeToEarth;
                isOnEarth = true;
            }
            double currentHeight = c_initialHeight - c_EarthG*seconds*seconds/2.0;
            double currentSpeed = c_EarthG*seconds;
            output("after "+qnum(seconds)+" sec: h = "+
                   QString::number((currentHeight<0.0) ? 0.0 : currentHeight, 'f', 2)+" m, V = "+
                   QString::number(currentSpeed, 'f', 2)+" m/s");
            return isOnEarth;
        }};

    for(int countSeconds=0; calculateHeight_TrueOnEarth(static_cast<double>(countSeconds))==false; ++countSeconds);
#endif
    output("---=== test 02 ===---");
    srand(time(nullptr));
    static int countTrys{0};
    static int hideNumber{0};
    const int c_countTrys{7};
    if(countTrys==0){
        hideNumber = rand()%100+1;
        output("--==Start Game! "+qnum(c_countTrys)+" trys!");
    }
    ++countTrys;
    int userNumber = ui->lineEdit_lesson63->text().toInt();
    if(userNumber==hideNumber){
        output("Yahoo! You are WIN! Number is "+qnum(hideNumber));
        countTrys = 0;
    }
    else if(userNumber>hideNumber){
        output("Number "+qnum(userNumber)+" HI then secret number ("+qnum(c_countTrys-countTrys)+" tries remain");
    }
    else{
        output("Number "+qnum(userNumber)+" LO then secret number ("+qnum(c_countTrys-countTrys)+" tries remain");
    }
    if(countTrys==c_countTrys){
        output("You lost! Secret number was "+qnum(hideNumber)+"! Try again");
        countTrys = 0;
    }
}

#include <QDateTime>

//lesson74-...
void Form_cpp_1::on_pushButton_9_clicked()
{
    output("---=== lesson 77 ===---");
    srand(time(nullptr));
    const int c_arraySize{1000000};
    //    std::array<std::pair<std::string, int>, c_arraySize> arr{};
    std::vector<std::pair<std::string, int>> arr{};
    quint64 tBeforeInitArrayMs = QDateTime::currentMSecsSinceEpoch();
    for (size_t i=0; i<c_arraySize; ++i) {
        int randomVal = rand()%100000;
        arr.push_back({std::to_string(randomVal)+"_name", randomVal});
//        arr[i].first = "name_"+std::to_string(i);
//        arr[i].second = rand()%100000;
    }
    quint64 tAfterInitArrayMs = QDateTime::currentMSecsSinceEpoch();
    auto l_printArray{ [this]<typename T>(const T& arr) {
            QString s{};
            bool isBigArray = false;
            for (size_t i=0; i<arr.size(); ++i) {
                if( i<20 || i>=arr.size()-20 ){
                    s += QString::fromStdString(arr[i].first)+"("+qnum(arr[i].second)+")  ";
                }
                else{
                    if( !isBigArray )
                        s += " ... ";
                    isBigArray = true;
                }
            }
            output(s);
        }};
    l_printArray(arr);
    quint64 tBeforeSortArrayMs = QDateTime::currentMSecsSinceEpoch();
    std::sort(arr.begin(), arr.end(), [](auto& a, auto& b) -> bool {
        return a.first < b.first;
    });
    quint64 tAfterSortArrayMs = QDateTime::currentMSecsSinceEpoch();
    output("-------========= after sort ======------");
    l_printArray(arr);
    output("array size "+qnum(arr.size())+" (sizeof "+qnum(sizeof(arr))+"), init "+qnum(tAfterInitArrayMs-tBeforeInitArrayMs)+" ms, sort "+
           qnum(tAfterSortArrayMs-tBeforeSortArrayMs)+" ms");
    output("---=== lesson 78-83 ===---");
    int arr78[3][5] = {
        { 0 },
        { 0 },
        { 0 }
    };
    int arr78_2[3] = {};
    using IntArray5_t = int[5];
    IntArray5_t *ptrArr78 = arr78;
    int *ptrArr78_2 = arr78_2;
    ptrArr78_2[1] = 50;
    ptrArr78[2][3] = 5;

    QString s78_2 = "";
    for(auto a: arr78_2){
        s78_2 += qnum(a)+"   ";
    }
    output(s78_2);

    s78_2 = "";
    for(size_t i=0; i<sizeof(arr78_2)/sizeof(int); ++i){
        s78_2 += qnum(ptrArr78_2[i])+"   ";
    }
    output(s78_2);

    std::string s78 = "12345678901234567890123456789012";
    output("string >"+QString::fromStdString(s78)+"< length is "+qnum(s78.length())+
           ", capacity is "+qnum(s78.capacity())+", sizeof is "+qnum(sizeof(s78)));
    char s78_1[] = "test string";
    output("size of test string is "+qnum(sizeof(s78_1)));
    int arr83[4][4] = {
        {11, 21, 31, 41},
        {12, 22, 32, 42},
        {13, 23, 33, 43},
        {14, 24, 34, 44},
    };
    int *ptrArr83 = &arr83[0][0];
    QString s83 = "";
    for(size_t i=0; i<4*4; ++i, ++ptrArr83){
        s83 += qnum( *ptrArr83 ) + "   ";
        if((i+1)%4 == 0)
            s83 += "\n";
    }
    output(s83);
    const char *c_str1 = "constant string";
    output(QString::fromStdString(c_str1));
    int *ptrInt = new (std::nothrow) int[10] {1,2,3, 0, 0, 0, 4};
    QString s86 = "";
    for(size_t i=0; i<10; ++i, ++ptrInt){
        s86 += qnum( *ptrInt ) + "   ";
    }
    output(s86);
    output("---=== lesson 93 ===---");
    int **ptrPtrArray = new (std::nothrow) int*[5];
    for(size_t i=0; i<5; ++i){
        ptrPtrArray[i] = new (std::nothrow) int[5+i] {};
    }
    for(size_t i=0; i<5; ++i){
        output("size of "+qnum(i)+" row is "+qnum(sizeof(ptrPtrArray[i])));
        QString s93 {};
        for(size_t j=0; j<5+i; ++j){
            s93 += qnum(ptrPtrArray[i][j])+"   ";
        }
        output(s93);
    }
//    std::array<int, 10> arr94 {};
//    arr94[11] = 11; //real ERROR? but no control
//    arr94.at(11) = 112; //real ERROR and throw exeption std::out_of_range
    output("---=== lesson 95 ===---");
    std::vector<int> vec95 {1,2,3};
    output("vector size is "+qnum(vec95.size())+", capacity is "+qnum(vec95.capacity()));
    vec95.reserve(12);
    output("(after reserve) vector size is "+qnum(vec95.size())+", capacity is "+qnum(vec95.capacity()));
    vec95.push_back(13);
    output("(after push_back) vector size is "+qnum(vec95.size())+", capacity is "+qnum(vec95.capacity()));
}

void print96(int p1, int p2)
{
    Form_cpp_1::outputS("from print96 ("+qnum(p1)+", "+qnum(p2)+")");
}

inline void print96_inline(int p1, int p2)
{
    Form_cpp_1::outputS("from print96 ("+qnum(p1)+", "+qnum(p2)+")");
}

const char* returnConstStringC()
{
    return "this is const C-string (return reference)";
}

const std::string returnConstStringS()
{
    return "this is const C++string (return copy string)";
}

int GetMax(const int a, const int b)
{
    return (a>b)?a:b;
}

inline int GetMaxInline(const int a, const int b)
{
    return (a>b)?a:b;
}

#define NDEBUG
#include <cassert>

//lesson96-...
void Form_cpp_1::on_pushButton_10_clicked()
{
    output("---=== lesson96-100 ===---");
    auto l_print96 {[this] (const QString& s) -> int {output(s);return 0;} };
    print96(l_print96("print p1"), l_print96("print p2"));
    const char *cstr = returnConstStringC();
    output(returnConstStringC());
    output(QString::fromStdString(returnConstStringS()));
    std::vector<int>vec100;
    vec100.reserve(1000000);
    output(cstr);
    const int &iref = 5;
    output(qnum(iref));
    print96_inline(1, 2);
    output("---=== lesson101 ===---");
    auto l_max { [] (const int a, const int b) -> int { return (a>b)?a:b;}};
    srand(time(nullptr));
    std::vector<int> vec101_1;
    std::vector<int> vec101_2;
    const size_t c_vectorSize{10*1000*1000};
    quint64 tStartMs = 0;
    quint64 tEndMs = 0;

    tStartMs = QDateTime::currentMSecsSinceEpoch();
    for(size_t i=0; i<c_vectorSize; ++i){
        vec101_1.push_back(rand()%1000000);
    }
    tEndMs = QDateTime::currentMSecsSinceEpoch();
    output("init vector - "+qnum(tEndMs-tStartMs)+" ms");
    vec101_2.reserve(c_vectorSize);
    tStartMs = QDateTime::currentMSecsSinceEpoch();
    for(size_t i=0; i<c_vectorSize; ++i){
        vec101_2.push_back(rand()%1000000);
    }
    tEndMs = QDateTime::currentMSecsSinceEpoch();
    output("init vector after reserve - "+qnum(tEndMs-tStartMs)+" ms");
    output("RAND_MAX is "+qnum(RAND_MAX));
    vec101_2.clear();
    output("vec101_2 size is "+qnum(vec101_2.size())+", capacity is "+qnum(vec101_2.capacity()));
    //l_max
    tStartMs = QDateTime::currentMSecsSinceEpoch();
    for(size_t i=0; i<c_vectorSize; ++i){
        vec101_2.push_back(l_max(vec101_1[i], 500000));
    }
    tEndMs = QDateTime::currentMSecsSinceEpoch();
    output("l_max time - "+qnum(tEndMs-tStartMs)+" ms");
    vec101_2.clear();
    //GetMax
    tStartMs = QDateTime::currentMSecsSinceEpoch();
    for(size_t i=0; i<c_vectorSize; ++i){
        vec101_2.push_back(GetMax(vec101_1[i], 500000));
    }
    tEndMs = QDateTime::currentMSecsSinceEpoch();
    output("GetMax time - "+qnum(tEndMs-tStartMs)+" ms");
    vec101_2.clear();
    //GetMaxInline
    tStartMs = QDateTime::currentMSecsSinceEpoch();
    for(size_t i=0; i<c_vectorSize; ++i){
        vec101_2.push_back(GetMaxInline(vec101_1[i], 500000));
    }
    tEndMs = QDateTime::currentMSecsSinceEpoch();
    output("GetMaxInline time - "+qnum(tEndMs-tStartMs)+" ms");
    vec101_2.clear();

    output("---=== lesson104 ===---");
    int (*getMaxPtr)(const int a, const int b) = GetMax;
    output(qnum(getMaxPtr(5,7)));
    getMaxPtr = GetMaxInline;
    output(qnum(getMaxPtr(9,7)));
    getMaxPtr = l_max;
    output(qnum(getMaxPtr(9,12)));
    auto getMaxPtr2 = GetMax;
    output(qnum(getMaxPtr2(15,12)));
    std::function<int(const int,const int)> getMaxPtr3 = l_max;
    output(qnum(getMaxPtr3(15,19)));
    output("---=== lesson104 (calculator) ===---");
    auto l_add{ [](const int a, const int b) -> int {
            return a+b;
        }};
    auto l_sub{ [](const int a, const int b) -> int {
            return a-b;
        }};
    auto l_mul{ [](const int a, const int b) -> int {
            return a*b;
        }};
    auto l_div{ [](const int a, const int b) -> int {
            return a/b;
        }};
    auto l_getOperFuncPtr{ [&](const mytypes::Operators oper) ->std::function<int(const int, const int)> {
            switch (oper) {
            case mytypes::Add:
                return l_add;
            case mytypes::Odd:
                return l_sub;
            case mytypes::Mult:
                return l_mul;
            case mytypes::Div:
                return l_div;
            default:
                return nullptr;
            }
        }};
    auto l_getOperName{ [](const mytypes::Operators oper) -> QString {
            switch (oper) {
            case mytypes::Add:
                return QString(" + ");
            case mytypes::Odd:
                return QString(" - ");
            case mytypes::Mult:
                return QString(" * ");
            case mytypes::Div:
                return QString(" / ");
            default:
                return QString(" n/a ");
            }
        }};
    const int a = ui->lineEdit_numA->text().toInt();
    const int b = ui->lineEdit_numB->text().toInt();
    mytypes::Operators oper = static_cast<mytypes::Operators>(ui->comboBox_operator->currentIndex());
    std::function<int(const int, const int)> operFuncPtr = l_getOperFuncPtr(oper);
    output(qnum(a)+l_getOperName(oper)+qnum(b)+" = "+qnum(operFuncPtr(a,b)));
    output("---=== lesson107 ===---");
    auto l_fibonachi { [] (const size_t number) -> std::vector<int> {
            std::vector<int> vec;
            for(size_t i=0; i<=number; ++i){
                if(i==0)
                    vec.push_back(0);
                else if(i==1)
                    vec.push_back(1);
                else{
                    vec.push_back(vec.at(i-1)+vec.at(i-2));
                }
            }
            return vec;
        }};
    std::vector<int> vec107 = l_fibonachi(15);
    QString s107("fibonachi for 15: ");
    for(auto i : vec107){
        s107 += qnum(i)+"  ";
    }
    output(s107);
    output("---=== lesson107 (test 2) ===---");
    std::function<int(const int)> l_10x{ [&l_10x](const int value) -> int {
            if( value < 10  ) return value;
            return l_10x(value/10)+value%10;
        }};
    auto l_print10xSum{ [this,l_10x](const int value) {
            output("sum numbers of "+qnum(value)+" equal "+qnum(l_10x(value)));
        }};
    l_print10xSum(482);
    l_print10xSum(83569);
    std::function<std::string(const int)> l_2x{ [&l_2x](const int value) -> std::string {
            if( value == 0  ) return "0";
            if( value == 1  ) return "1";
            return l_2x(value/2)+(value%2 ? "1" : "0");
        }};
    auto l_print2xSum{ [this,l_2x](const int value) {
            output("sum numbers of "+qnum(value)+" equal "+QString::fromStdString(l_2x(value)));
        }};
    l_print2xSum(12);
    l_print2xSum(37);
    l_print2xSum(15);
    l_print2xSum(30);
    assert(b>0 && "lineEdit_numB can't be zero"); //assert disabled by define NDEBUG
//    static_assert (sizeof(long)==4);
    output("---=== chapter 7 test ===---");

    std::vector<std::pair<int, int>> vecPair;
//    auto l_pairCompare{ [](const std::pair<int, int>& a, const std::pair<int, int>& b) -> bool {
//            return a.second < b.second;
//        }};
    auto l_pairCompare{ [](const auto& a, const auto& b) -> bool {
            return a.second < b.second;
        }};
    auto l_pairBSearchPair{ [](const auto& a, const auto& b) -> bool {
            return a.second < b.second;
        }};
//    auto l_pairBSearch{ [](const auto& element, const int& value) -> bool {
//            return element.second < value;
//        }};
    for(size_t i=0; i<c_vectorSize; ++i){
        vecPair.push_back(std::pair<int, int>(i, rand()%10000000));
    }
    //sort
    tStartMs = QDateTime::currentMSecsSinceEpoch();
    std::sort(vecPair.begin(), vecPair.end(), l_pairCompare);
    tEndMs = QDateTime::currentMSecsSinceEpoch();
    output("sort vector - "+qnum(tEndMs-tStartMs)+" ms");
    std::vector<bool> isFoundVector;
    const size_t c_countSearch{100000};
    std::vector<std::pair<int, int>> vecPairFound;
    tStartMs = QDateTime::currentMSecsSinceEpoch();
    for(size_t i=0; i<c_countSearch; ++i){
        std::pair<int,int> value(0, 100000+i);
        auto foundIt = std::lower_bound(vecPair.begin(), vecPair.end(), value, l_pairBSearchPair);
        if( !(foundIt == vecPair.end()) && !l_pairBSearchPair(value, *foundIt) ){
            isFoundVector.push_back(true);
            vecPairFound.push_back(*foundIt);
        }
        else{
            isFoundVector.push_back(false);
        }
//        isFoundVector.push_back(
//        std::binary_search(vecPair.begin(), vecPair.end(), std::pair<int,int>(0, 100000+i), l_pairBSearchPair));
    }
    tEndMs = QDateTime::currentMSecsSinceEpoch();
    int countTrue{0};
    int countFalse{0};
    for(auto i: isFoundVector){
        if(i==true)
            ++countTrue;
        else
            ++countFalse;
    }
    output("search "+qnum(c_countSearch)+" times in vector - "+qnum(tEndMs-tStartMs)+" ms, result is true - "+
           qnum(countTrue)+" times, false - "+qnum(countFalse)+" times");
    output("print first 30 elements");
    int countPrintElem{0};
    for(auto i: vecPairFound){
        if(++countPrintElem > 30 )
            break;
        output("< "+qnum(i.first)+", "+qnum(i.second)+" >");
    }
}


class Chapter8
{
    int m_a;
    double m_b;
    QString m_c;
    const std::string m_e;
    static int m_f;
    static constexpr std::array<int, 5> m_arr {1,2,3,4,5};
    static std::vector<int> m_vec;
public:
    Chapter8(const int a = 12, const double b = 13.5, const QString& c = "qstring value", const std::string& e = "std string value")
        : m_a(a), m_b(b), m_c(c), m_e(e)
    {}
    Chapter8(const QString& c) : Chapter8()
    {
        m_c = c;
    }
    template <typename TPrintFunc>
    void print (TPrintFunc printFunc) const {
        QString s = "a - "+qnum(m_a)+", b - "+qnum(m_b)+", c - "+m_c+", e - "+QString::fromStdString(m_e)+", f - "+qnum(m_f);
        s += ", array{  ";
        for(auto i : m_arr){
            s += qnum(i)+"  ";
        }
        s+= "}";
        s += ", vector{  ";
        for(auto i : m_vec){
            s += qnum(i)+"  ";
        }
        s+= "}";
        printFunc(s);
    }
    int& returnA() {
        ++m_f;
        return m_a;
    }
    class static_initializer{
    public:
        static_initializer(){
            m_f = 10;
            srand(time(nullptr));
            for(size_t i=0; i<10; ++i)
                m_vec.push_back(rand()%100);
        }

    };
private:
    static static_initializer s_initializer;
};

int Chapter8::m_f;
std::vector<int> Chapter8::m_vec;
Chapter8::static_initializer Chapter8::s_initializer;

//friend
class Coord3D
{
protected:
    double m_x, m_y, m_z;
public:
    Coord3D(double x=0.0, double y=0.0, double z=0.0) : m_x(x), m_y(y), m_z(z){}
};

class Vector3D;

class Point3D : Coord3D
{
public:
    enum PointType{
        SIMPLE,
        DIFFICULT
    };

    Point3D(double x=0.0, double y=0.0, double z=0.0) :Coord3D(x, y, z) {}
    template<typename TPrintFunc>
    void print() const {
        TPrintFunc printfunc;
        printfunc("point("+qnum(m_x)+", "+qnum(m_y)+", "+qnum(m_z)+")");
    }
    const Point3D& moveByVector(const Vector3D &v);
};

class Vector3D : Coord3D
{
public:
    Vector3D(double x=0.0, double y=0.0, double z=0.0) :Coord3D(x, y, z) {}
    template<typename TPrintFunc>
    void print() const {
        TPrintFunc printfunc;
        printfunc("vector("+qnum(m_x)+", "+qnum(m_y)+", "+qnum(m_z)+")");
    }
    friend const Point3D& Point3D::moveByVector(const Vector3D &v);
};

const Point3D& Point3D::moveByVector(const Vector3D &v){
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
    return *this;
}

#include <chrono>

class MyTimer
{
    using clock_t = std::chrono::steady_clock;
    using second_t = std::chrono::duration<double>;
    using milli_t = std::chrono::duration<int64_t, std::milli>;
    using micro_t = std::chrono::duration<int64_t, std::micro>;
    std::chrono::time_point<clock_t> m_begin;
public:
    MyTimer() : m_begin(clock_t::now()) {}
    void reset(){
        m_begin = clock_t::now();
    }
    double elapsed_s(){
        return std::chrono::duration_cast<second_t>(clock_t::now()-m_begin).count();
    }
    int64_t elapsed_ms(){
        return std::chrono::duration_cast<milli_t>(clock_t::now()-m_begin).count();
    }
    int64_t elapsed_mks(){
        return std::chrono::duration_cast<micro_t>(clock_t::now()-m_begin).count();
    }
};

//chapter8
void Form_cpp_1::on_pushButton_11_clicked()
{
    Chapter8 ch1;
    Chapter8 ch2("my C");
    Chapter8 ch3(1, 2.5, "CCCC C", "EEEE EE");
    ch1.print(outputS);
    ch2.print(outputS);
    ch3.print(outputS);
    int &a = ch3.returnA();
    a = 5; //BAD style
    ch3.print(outputS);
//    Chapter8::m_f = 10; //ERROR: private member
    ch1.print(outputS);
    ch2.print(outputS);
    ch3.print(outputS);
    output("---=== lesson 127 ===---");
    Point3D p(1,2,3);
    Vector3D v(2.5,3.5,4.5);
    p.print<outputHelper>();
    v.print<outputHelper>();
    p.moveByVector(v);
    p.print<outputHelper>();
    Point3D(4,6,8).print<outputHelper>();
    Point3D(5,5,5).moveByVector(Vector3D(5,6,7.5)).print<outputHelper>();
//    Point3D::PointType pointType = Point3D::DIFFICULT;
    std::vector<double> vec129;
    MyTimer time;
    srand(std::time(nullptr));
    time.reset();

//    qint64 tStartMs = QDateTime::currentMSecsSinceEpoch();
    for(size_t i=0; i<10000000; ++i){
        vec129.push_back(rand());
    }
//    qint64 tEndMs = QDateTime::currentMSecsSinceEpoch();
//    output("init vector - "+qnum(tEndMs-tStartMs)+" ms");
    double timeDiff_s = time.elapsed_s();
    int64_t timeDiff_ms = time.elapsed_ms();
    int64_t timeDiff_mks = time.elapsed_mks();
    output("vec init: "+qnum(timeDiff_s)+" s, "+qnum(timeDiff_ms)+" ms, "+qnum(timeDiff_mks)+" mks");
    output("vec init (get and print time together): "+qnum(time.elapsed_s())+" s, "+qnum(time.elapsed_ms())+" ms, "+qnum(time.elapsed_mks())+" mks");

    time.reset();
    std::sort(vec129.begin(), vec129.end());
    timeDiff_ms = time.elapsed_ms();
    output("vec sort "+qnum(timeDiff_ms)+" ms");
}

















//8Start
void Form_cpp_1::on_pushButton_8Start_clicked()
{
    output("Deck:");
    m_deck.print<outputHelper>();
    output("Deck after shuffle:");
    m_deck.ShuffleDeck();
    m_deck.print<outputHelper>();
    m_player.resetValues();
    m_dealer.resetValues();
    m_dealer.addCard(*m_deck.dealCard<outputHelper>());
    m_dealer.printValues<outputHelper>();
    m_player.addCard(*m_deck.dealCard<outputHelper>());
    m_player.addCard(*m_deck.dealCard<outputHelper>());
    m_player.printValues<outputHelper>();
    if(m_player.isLoss()){
        output("<Player> LOSS!");
        m_player.resetValues();
        m_dealer.resetValues();
    }
    ui->pushButton_8Start->setEnabled(false);
    ui->pushButton_8Get->setEnabled(true);
    ui->pushButton_8Stop->setEnabled(true);
}

//8Get
void Form_cpp_1::on_pushButton_8Get_clicked()
{
    m_player.addCard(*m_deck.dealCard<outputHelper>());
    m_player.printValues<outputHelper>();
    if(m_player.isLoss()){
        output("<Player> LOSS!");
        ui->pushButton_8Start->setEnabled(true);
        ui->pushButton_8Get->setEnabled(false);
        ui->pushButton_8Stop->setEnabled(false);
    }
}

//8Stop
void Form_cpp_1::on_pushButton_8Stop_clicked()
{
    while(!m_dealer.isDealerEnough()){
        m_dealer.addCard(*m_deck.dealCard<outputHelper>());
        m_dealer.printValues<outputHelper>();
    }
    if(m_dealer.isLoss()){
        output("<Player> WIN!");
    }
    else if(m_player.isIWin(m_dealer)){
        output("<Player> WIN!");
    }
    else{
        output("<Player> LOSS!");
    }
    ui->pushButton_8Start->setEnabled(true);
    ui->pushButton_8Get->setEnabled(false);
    ui->pushButton_8Stop->setEnabled(false);
}

void Form_cpp_1::on_pushButton_chapter9_clicked()
{

}


















