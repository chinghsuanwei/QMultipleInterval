#include <QCoreApplication>

#include <iostream>

#include <QVector>
#include <QString>

class Interval
{
public:
    Interval() :
        start(0),
        end(0)
    {

    }

    Interval(unsigned int start, unsigned int end) :
        start(start),
        end(end)
    {

    }

    unsigned int size()
    {
        return end-start+1;
    }

    unsigned int start;
    unsigned int end;
};

class QMultipleInterval
{
public:
    QMultipleInterval() {}
    static QMultipleInterval read(QString q_str)
    {
        QMultipleInterval intervals;
        QStringList list = q_str.split(",");

        foreach(const QString& segment ,list)
        {
            int idx = segment.indexOf("-");
            int start = 0;
            int end = 0;
            if(idx == -1)
            {
                start = segment.toInt();
                end = start;
            }
            else
            {
                bool ok = false;
                start = segment.mid(0, idx).toInt(&ok);
                //throw parse error
                end = segment.mid(idx+1).toInt(&ok);
                //throw parse error
            }
            intervals.ranges.push_back(Interval(start, end));
        }

        return intervals;
    }

    class iterator
    {
        public:
            typedef QMultipleInterval* pointer;
            iterator(pointer ptr) : ptr_(ptr), n(0), offset(0) { }
            iterator operator++() {

                if(ptr_ == nullptr) return *this;

                if(ptr_->ranges[n].size() <= (offset+1) )
                {
                    n++;
                    if(ptr_->ranges.size() <= n)
                    {
                        ptr_ = nullptr;
                        n = 0;
                    }
                    offset = 0;
                }
                else
                {
                    offset++;
                }

                return *this;
            }

            int operator*() {
                return ptr_->ranges[n].start + offset;
            }

            bool operator!=(QMultipleInterval::iterator& rhs) {
                return ptr_ != rhs.ptr_ || n != rhs.n || offset !=rhs.offset;
            }
        private:
            iterator(pointer ptr, int arg1, int arg2) : ptr_(ptr), n(arg1), offset(arg2) {
            }
        public:
            int n;
            int offset;
            QMultipleInterval* ptr_;
    };

    iterator begin()
    {
        return iterator(this);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    unsigned int size();
public:
    QVector<Interval> ranges;
};

int main()
{
    QString str = "1-9,16,27-29";

    QMultipleInterval intervals = QMultipleInterval::read(str);
    intervals.ranges.push_back(Interval(0, 3));
    intervals.ranges.push_back(Interval(7, 9));

    for(QMultipleInterval::iterator i = intervals.begin(); i != intervals.end(); i++)
    {
        std::cout << *i << " ";
    }

    for(int i : intervals)
    {
        std::cout << i << " ";
    }

    return 0;
}
