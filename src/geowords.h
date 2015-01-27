#ifndef GEOWORDS_H
#define GEOWORDS_H

#include "globj.h"
#include <QString>
#include "box2.h"

class GeoWords : public GlObj
{
public:

    enum BoxCompute
    {
        BoxCompute_Auto = 1,
        BoxCompute_LRT,
        BoxCompute_LRB,
        BoxCompute_TLBR
    };

    struct WordPos
    {
        std::string word;
        box2d box;
        double fontSz;
        char orient;

        WordPos()
        {
            fontSz = 0;
            orient = 'h';
        }

        void join(const WordPos &wp)
        {
            // set the orient
            if (word.size() <= 1)
            {
                orient = wp.orient;
            }

            if (word.size() <= 0)
            {
                word = wp.word;
            }
            else
            {
                // - means the word was cut off
                if (word.at(word.size()-1) == '-')
                {
                    word.erase(word.size() - 1);
                }
                else
                {
                    word += " ";
                }

                word += wp.word;
            }


            box.update(wp.box);
        }
    };

    // to find bounding box
    struct WordFind
    {
        std::string word;
        char orient;

        WordFind()
        {
            orient = 'h';
        }

        WordFind(const std::string &w, const std::string &o)
        {
            set(w, o);
        }

        void set(const std::string &w, const std::string &o)
        {
            word = w;
            orient = 'h';
            if (o.size() <= 0) return;

            if (o[0] == 'v' || o[0] == 'V' )
            {
                orient = 'v';
            }
        }

        bool isOrientH() const
        {
            return orient == 'h';
        }

        bool isOrientV() const
        {
            return orient == 'v';
        }
    };

protected:
    enum Side
    {
        Left,
        Top, // NOTE: IN OUR WORD COORDS, TOP is Larger Than BTM (cartesian coordinates)
        Right,
        Btm
    };


public:
    GeoWords ();

    bool load(const char *file);

    virtual void computeBox(DrawData *pdd, box3d *pbox);

    void setWordTopLeft(const std::string &word, std::string orient="h") { _wordTL.set(word, orient); }
    void setWordBtmRight(const std::string &word, std::string orient="h") { _wordBR.set(word, orient); }
    void setWordLeft(const std::string &word, std::string orient="h") { _wordL.set(word, orient); }
    void setWordRight(const std::string &word, std::string orient="h") { _wordR.set(word, orient); }
    void setWordTop(const std::string &word, std::string orient="h") { _wordT.set(word, orient); }
    void setWordBtm(const std::string &word, std::string orient="h") { _wordB.set(word, orient); }
    void setOffsetFromTop(double ofs) { _autoOfsTnB = ofs; } // this offset is used from the bottom of the words bounding box

    std::vector<WordPos>& getWordsValid() { return _wordsValid; }
    std::vector<WordPos>& getWordsJoined() { return _wordsJoined; }
    std::vector<WordPos>& getWordsSections() { return _wordsSections; }

    void setComputeBoxMode(BoxCompute mode) { _boxCompute = mode; }
    BoxCompute getComputeBoxMode() { return _boxCompute; }
    bool isComputeBoxAuto() { return _boxCompute == BoxCompute_Auto; }

    bool updateBoxHeightIfNeeded(double h);

protected:
    bool loadWord(const QString &line, int linenum);
    bool isRemoveType(const WordPos &wp);
    bool isDup(const WordPos &wp);
    bool isSection(const WordPos &wp);
    void joinWords();

    bool computeWordBox();
    bool computeWordBoxAuto();
    bool computeWordBoxLRT();
    bool computeWordBoxLRB();
    bool computeWordBoxTLBR();


    std::string toString(Side side);
    void setBox(const WordPos &wp, const WordFind &wf, Side side);
    WordPos* findWord(const WordFind &w, Side side);
    WordPos* findWordL();
    WordPos* findWordR();
    WordPos* findWordT();
    WordPos* findWordB();
    WordPos* findWordTL();
    WordPos* findWordBR();
    WordPos* findTN();
    WordPos* findRW();

protected:
    bool _removeNumeric;
    unsigned int _minWordLen;
    std::vector<std::string> _removeList;

    std::string _commaEscape;

    BoxCompute _boxCompute;
    double _autoOfsL;
    double _autoOfsR;
    double _autoOfsTnB;
    WordFind _wordTL;
    WordFind _wordBR;
    WordFind _wordL;
    WordFind _wordR;
    WordFind _wordT;
    WordFind _wordB;
    box2d _box;

    std::vector<WordPos> _words;
    std::vector<WordPos> _wordsValid;
    std::vector<WordPos> _wordsJoined;
    std::vector<WordPos> _wordsSections;

    // it would be easier to look for section font color, but so far unable to find that with clown pdf,
    // but font size seems unique to section numbers in current pdfs.
    int _sectionMin;
    int _sectionMax;
   // double _sectionFontSize;
    std::vector<double> _sectionFontSizes;
};

#endif
