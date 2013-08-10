#ifndef QSTATISTICS_P_H
#define QSTATISTICS_P_H

template <typename T>
class QStatistics : public QList< T >
{
public:
    QStatistics() {}

    T average() const {
        if (this->isEmpty()) return -1;
        T sum = 0;
        foreach (T d, *this) {
            sum += d;
        }

        return sum / this->count();
    }

    T variance() const {
        if (this->isEmpty()) return -1;
        T ave = average();
        T sum = 0;
        foreach (T d, *this) {
            sum += (d - ave) * (d - ave);
        }

        return sum / this->count();
    }

    T standardDeviation() const {
        return qSqrt(variance());
    }

    T min() const {
        if (this->isEmpty()) return -1;
        T ret = this->at(0);
        foreach (T d, *this) {
            ret = qMin(d, ret);
        }
        return ret;
    }

    T max() const {
        if (this->isEmpty()) return -1;
        T ret = this->at(0);
        foreach (T d, *this) {
            ret = qMax(d, ret);
        }
        return ret;
    }
};

#endif // QSTATISTICS_P_H
