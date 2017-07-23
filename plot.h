#include <vector>
#include <string>
#include <stdlib.h>

#include <gd.h>
#include <gdfonts.h>

typedef struct gdImageStruct* gdImagePtr;

class Plot {
    public:
        Plot(int width, int height, int totalSeconds, int samplingPeriod);
        ~Plot();
        std::string plotCpu(const std::vector<double>& data);

    private:
        std::string toPng();
        int getX(ssize_t x, ssize_t total) const;
        int getY(double value) const;
        void label(double maxValue);

        struct MyGdFont;
        typedef struct MyGdFont* MyGdFontPtr;

        const int width_;
        const int height_;
        const int totalSeconds_;
        const int samplingPeriod_;
        const gdImagePtr image_;
        const MyGdFontPtr font_;
        const int fontWidth_;
        const int fontHeight_;
        const int background_;
        const int black_;
        const int gray_;
        const int blue_;

        const int kRightMargin_;
        static const int kLeftMargin_ = 5;
        static const int kMarginY_ = 5;

        const double ratioX_;
};

