/**
 * @file KDataFrame.h
 * @brief pandas module like CSV parser
 * @author K. Kotera
 * @date 2024-11-01
 * @version b 1.0.1
 *
 * @details pythonのpandasモジュール的なことができることを目指して開発が始まったCSVパーサ.
 * @note めっちゃ開発段階. C++17.
 * @par バージョン履歴
 * - b 1.0.1 (2024-11-01) 対応バージョンの修正C++14->C++17. 今後C++14に対応予定.
 * - b 1.0.0 (2024-10-30 - 2024-10-31) 初期リリース.
*/
#ifndef __KDATAFRAME_H__
#define __KDATAFRAME_H__

#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <map>
#include <string>
#include <iomanip>
#include <bitset>

#include <iostream>
#include <fstream>
#include <sstream>

#include <regex>
#include <variant>

using tvString = std::vector<std::string>;
using tKeyMap  = std::map          <std::string, int>;
using tKeyUMap = std::unordered_map<std::string, int>;

#if __cplusplus > 201402L
using KVector = std::variant<int, double, std::string>;
/**
 * @brief 文字列を自動的に整数、浮動小数点へキャストします.
 * @param inp_val[in] 変換元の文字列
 * @return 自動キャストされた値[int, double, string].
 * @details 文字列のパターンから自動的に整数、浮動小数点へ変換します.
 * 変換できなかった場合変換は行われず、std::stringとして返します.
 * @note std::variantがC+17以降はこちらの関数が機能します.
 * 詳細は\ref T StringTo(const std::string &inp_val)を参照してください.
*/
KVector StringTo(const std::string &inp_val)
{
    // 正規表現
    std::regex IntPattern("^-?\\d+$");                    // 整数
    std::regex FloatPattern("^-?\\d*\\.\\d+$");           // 浮動小数点
    std::regex ExpPattern("^-?\\d*\\.?\\d+[eE][-+]?\\d+$"); // 指数表記

    if (std::regex_match(inp_val, IntPattern)) 
    {
        return std::stoi(inp_val);  // 整数に変換
    } 
    else if (std::regex_match(inp_val, FloatPattern)) 
    {
        return std::stod(inp_val);  // 浮動小数点数に変換
    } 
    else if (std::regex_match(inp_val, ExpPattern)) 
    {
        return std::stod(inp_val);  // 指数表記を浮動小数点数に変換
    } 
    else return inp_val; // どれにも該当しない場合は文字列として返す
}
#else
/**
 * @brief 文字列を指定の型へ変換します
 * @param inp_val[in] 変換元の文字列
 * @return キャストされた値(int, double).
 * @note C++14用に実装されました.C++17のStringTo関数はstd::variantによって「自動的に」文字列のキャストが行われます.
*/
template<class T> T StringTo(const std::string &inp_val)
{
    T Result;
    std::stringstream StrStr(inp_val);
    
    StrStr>>Result;

    // // 変換が失敗、もしくは変換後にデータが残っている場合は例外を投げる
    // if (ss.fail() || !ss.eof()) {
    //     throw std::invalid_argument("Conversion failed for input: " + inp_val);
    // }
    return Result;
}
/**
 * @brief 文字列を指定の型へ変換します
 * @param inp_val[in] 変換元の文字列
 * @return キャストされた値(int, double).
 * @details 文字列に対する関数templateの部分特殊化.
 * @note C++14用に実装されました.C++17のStringTo関数はstd::variantによって「自動的に」文字列のキャストが行われます.
*/
template <>
std::string StringTo<std::string>(const std::string& inp_val)
{
    return inp_val;
}
#endif

/**
 * @brief std::vectorに対する標準出力の<<演算子のオーバーロード.
 * @details std::vectorに対して<<演算子はオーバーロードされていないので実装.
*/
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
    for (int i=0; i<vec.size(); i++)
    {
        os<<vec[i];
        if (i != vec.size()-1) os<<", ";
    }
    return os;
}

/**
 * @brief CSVファイルの情報を記録
 * @details CSVファイルにおける、コメント、カラム名、全データのイベント数を記録.
*/
struct KCsvStruct
{
    // private:
    // protected:
    public:
        // Comment
        int nSkip = 0;
        //! コメントを保存
        std::vector<std::string> Comments;
        // columnName
        //! カラム名を保存
        std::vector<std::string>    columnName;
        std::map<std::string, int> mcolumnName;
        // data
        int Events;
        // Table
        //! テーブルの幅の最大値
        int TableWidthMax = 100;
        std::vector<int> WidthcolumnName;
        std::vector<int> WidthData;
        std::vector<int> WidthTableCols;

    public:
        /**
        * @brief カラムのキーを取得
        * @return カラムの名前をstd::vector<std::string>で返します
        */
        std::vector<std::string> Keys() const {return columnName;}
};


/**
 * @brief KDataFrame class
 * CSVパーサの制御クラス. ここだけ触っていればおｋ.
 * @details KDataFrameの本体.実際の操作はこのクラスを使う.
*/
class KDataFrame
{
    public:
        /** @brief デフォルトコンストラクタ*/
        KDataFrame(std::string path="", bool columnName=true);
        /** @brief デストラクタ*/
        ~KDataFrame();
    private:
        std::string Path;
    public:
        /** @brief ファイル名を指定します*/
        void SetFilename(std::string &path){Path=path;}

    private:
        std::map<std::string, unsigned> mRegTag;
        unsigned fReg;
        char Delimiter = ',';

        KCsvStruct     DFInfo;
        std::ifstream InpFile;
    public:
        void Init(); 
        void Open();
        void Close();
    public:
        void Scan(std::string col_list="", int events=-2, int width=-1);
        /** @brief 表の横幅を指定します.*/
        void SetTableWidth(int width=100){DFInfo.TableWidthMax=width;} 
        /** @brief 全データのイベント数を数えます.*/
        int  GetEntries() const {return DFInfo.Events;}
    // 値の取得関係
    public:
        /** @brief CSVファイルの基本情報をKCsvStruct class形式で取得します.*/
        KCsvStruct GetDFInfo() const {return DFInfo;}
        std::vector<std::string> GetcolumnStr(std::string column);
        template<class T> std::vector<T> Get(std::string column);
        std::vector<std::string> operator[] (const std::string column);
};


/**
 * @brief 引数付きデフォルトコンストラクタ
*/
KDataFrame::KDataFrame(std::string path, bool columnName)
{
    KDataFrame::Init();

    if(columnName) fReg |= mRegTag["columnNameEnable"];

    if (!path.empty()) 
    {
        KDataFrame::SetFilename(path);
        KDataFrame::Open();
    }
}

/**
 * @brief デストラクタ
*/
KDataFrame::~KDataFrame()
{
    KDataFrame::Close();
}

/**
 * @brief 初期化関数
 * @details デフォルトコンストラクタ実行時に一回だけ呼び出されます.フラグのデフォルト設定を行います.
*/
void KDataFrame::Init()
{


    auto Bit = [](unsigned bit_no){return (unsigned) 1 << (bit_no);};

    mRegTag["Open"] = Bit(0);
    mRegTag["columnNameEnable"] = Bit(1);
    mRegTag["DefinedColName"]  = Bit(2);

    // std::cout<<std::bitset<8>(mRegTag["Open"])<<"\n";
    // std::cout<<std::bitset<8>(mRegTag["columnNameEnable"])<<"\n";
    // std::cout<<std::bitset<8>(mRegTag["columnNameEnable"])<<"\n";

}

/**
 * @brief ファイルを開きます
 * @details デフォルトコンストラクタ実行時に一回だけ呼び出されます.ファイルの存在チェック後、中身を読み取ります.このときにファイルのコメントとカラム名、データの総数を記録します.
*/
void KDataFrame::Open()
{
    InpFile.open(Path);

    if(!InpFile.is_open())
    {
        std::cerr<<"Cannot open the file:"<<Path<<std::endl;
        return;
    }

    fReg |= mRegTag["Open"];
    
    std::string BufLine;
    unsigned Counter=0;
    while (std::getline(InpFile, BufLine))
    {
        // std::cout<<BufLine<<"\n";
        std::vector<std::string> Rows;
        std::stringstream SStream(BufLine);
        std::string Cells;

        if(BufLine=="\n") continue;

        if (BufLine[0] == '#')
        {
            DFInfo.Comments.push_back(BufLine);
            DFInfo.nSkip++;
            continue;
        }

        while (std::getline(SStream, Cells, Delimiter))  Rows.push_back(Cells);

        // std::cout<<"\naaaaa\n";
        // if(Counter==0) std::cout<<std::endl;
        // std::cout<<"\n";

        if(Counter == 0 ) // データの取得になるまでカウンターは進まない
        {
            DFInfo.WidthData.resize(Rows.size());
            DFInfo.WidthcolumnName.resize(Rows.size());
            DFInfo.WidthTableCols.resize(Rows.size());

            std::vector<std::string> ColNames;
            for(auto iColName=0; iColName<Rows.size(); iColName++)
            {
                DFInfo.WidthData[iColName] = 0;
                std::string ColName = "";
                
                if(fReg & mRegTag["columnNameEnable"])
                {
                    ColName = Rows[iColName];
                    if(DFInfo.mcolumnName.count(ColName)) 
                        ColName += "."+std::to_string(iColName);
                }
                else
                {
                    ColName = std::to_string(iColName);
                }
                ColNames.push_back(ColName);
                DFInfo.mcolumnName[ColName] = iColName;
                // std::cout<<iColName<<":"<<DFInfo.columnName[iColName]<<","<<DFInfo.mcolumnName[Rows[iColName]]<<"\n";
                // std::cout<<ColNames[iColName].size()<<"\n";
                DFInfo.WidthData[iColName] = (int) ColName.size();
                DFInfo.WidthcolumnName[iColName] = (int) ColName.size();
                // Rows.size();
                // WidthcolumnName
            }
            DFInfo.columnName = ColNames;

            // データの幅を保存するための配列を作成
            fReg |= mRegTag["DefinedColName"]; // カラム名を定義完了
            // std::cout<<"aaaaaaaa\n";
            DFInfo.nSkip++;
        }

        if((Counter > 0) && (fReg & mRegTag["DefinedColName"]))
        {
            for(auto i=0; i<Rows.size(); i++)
            {
                DFInfo.WidthData[i] = std::max(DFInfo.WidthData[i], (int) Rows[i].size());
            }
        }

        SStream.clear();
        Counter++;
    }

    DFInfo.Events = Counter;
    // std::cout<<"Event entries="<<GetEntries()<<std::endl;
}

/** 
 * @brief 読み込んだファイルの情報を一覧表示します.
 * @param[in] col_list 読み込むカラムを指定(未実装).
 * @param[in] events 表示する行数を指定(default=-2)
 * -2: 20行以下まで表示, -1: 全表示(未実装), その他は指定した行数まで
 * @param[in] width Tableの最大幅(default=-1: デフォルト値:100を使用)
 * @details だたし、非常に長いデータが格納されている場合スキップされます.スキップの基準はwidth変数によって指定できます.
*/
void KDataFrame::Scan(std::string col_list, int events, int width)
{
    if(width   >0) KDataFrame::SetTableWidth(width);
    if(events==-2) events = 20;

    int TotalWidth = 0;
    DFInfo.WidthTableCols.resize(DFInfo.WidthcolumnName.size());
    for(auto i=0; i<DFInfo.WidthcolumnName.size(); i++)
    {
        DFInfo.WidthTableCols[i] = std::max(DFInfo.WidthData[i], DFInfo.WidthcolumnName[i]);
        TotalWidth += DFInfo.WidthTableCols[i];
    }

    if(InpFile.is_open())
    {
        InpFile.clear();  
        InpFile.seekg(0); 

        int Counter = 0;
        std::string BufLine="";
        std::string TableLine="";
        while (std::getline(InpFile, BufLine))
        {
            // std::cout<<BufLine<<"\n";
            std::vector<std::string> Rows;
            std::stringstream SStream(BufLine);
            std::string Cells;

            std::stringstream RowTitle;
            RowTitle<<"| ";

            int ColNo = 0;
            while (std::getline(SStream, Cells, Delimiter))
            {
                if(TotalWidth>DFInfo.TableWidthMax)
                {
                    if(DFInfo.WidthTableCols[ColNo]>30) continue;
                }

                RowTitle<<std::setw(DFInfo.WidthTableCols[ColNo]+2)<<Cells<<" | ";
                ColNo++;
            }
            if(Counter==0)
            {
                for(int i=0; i<RowTitle.str().size(); i++) TableLine += "-";
                std::cout<<TableLine<<"\n";
                std::cout<<RowTitle.str()<<"\n";
                std::cout<<TableLine<<"\n";
            }
            else std::cout<<RowTitle.str()<<"\n";

            SStream.clear();
            if(events>0 && Counter>events) break;
            Counter++;
            // std::cout<<Counter<<"\n";
        }
        std::cout<<TableLine<<"\nEvent entries: "<<GetEntries()<<" events";
        if(GetEntries()>events) std::cout<<" (all events: \"events\" parameter set to be -1)";
        std::cout<<std::endl;
    }
}

/** 
 * @brief ファイルを閉じます.
 * @details デストラクタでファイルが閉じられていない場合、自動で呼び出されます.
*/
void KDataFrame::Close()
{

    if (InpFile.is_open())
    {
        InpFile.close();
        fReg &= ~mRegTag["Open"]; // flag cleared
    }
}

/** 
 * @brief 指定したカラムの値を任意の型で取り出します.
 * @param[in] column 取り出したいカラム名.
 * @return 指定した型のvector (std::vector<T>)
 * @details 失敗すると型変換のエラーが出ます.(多分)
*/
template<class T> std::vector<T> KDataFrame::Get(std::string column)
{
    std::vector<T> Vals;
    auto ValsStr = KDataFrame::GetcolumnStr(column);

    for(auto &Val: ValsStr) 
    {
        #if __cplusplus > 201402L
                auto ThisVal = StringTo(Val);
                T &ThisVal1 = std::get<T>(ThisVal);
                Vals.push_back(ThisVal1);
        #else
                auto ThisVal = StringTo<T>(Val);
                Vals.push_back(ThisVal);
        #endif
    }

    return Vals;
}

/** 
 * @brief 指定したカラムの値を文字列型で取り出します.
 * @param[in] column 取り出したいカラム名.
 * @return std::vector<std::string>
 * @details 失敗すると型変換のエラーが出ます.
*/
std::vector<std::string> KDataFrame::GetcolumnStr(std::string column)
{
    std::vector<std::string> Vals;
    int ColNo = DFInfo.mcolumnName[column];

    if (InpFile.is_open())
    {
        InpFile.clear();
        InpFile.seekg(0);

        int Counter = 0;
        std::string BufLine;
        // std::cout<<DFInfo.nSkip<<std::endl;

        while (std::getline(InpFile, BufLine))
        {
            // コメントとカラムはスキップ
            if (Counter<DFInfo.nSkip) {Counter++; continue;}

            std::stringstream SStream(BufLine);
            std::string Cells;

            int iCol=0;
            while (std::getline(SStream, Cells, Delimiter))
            {
                if(ColNo==iCol) Vals.push_back(Cells);
                iCol++;
            }
            SStream.clear();
            Counter++;
        }
    }
    else
    {
        std::cout<<"Error: CannotOpen the file"<<std::endl;
    }

    return Vals;
}

/** 
 * @brief 配列添字演算子[]のオーバーロード.
 * @param[in] column 取り出したいカラム名.
 * @return std::vector<std::string>
 * @details カラム名を指定することでカラムの値を文字列で取得できます(内部でKDataFrame::GetcolumnStr(std::string column)が呼び出される). e.g. auto f = df["aaaa"];
*/
std::vector<std::string> KDataFrame::operator[] (std::string column)
{
    return KDataFrame::GetcolumnStr(column);
}

#endif