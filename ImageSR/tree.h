#pragma once

#include "node.h"

namespace imgsr
{
    class TreeJsonException : std::exception
    {
    public:
        TreeJsonException(const char* msg_) :msg(msg_) {}
        const char* what() const { return msg.data(); }
    private:
        string msg;
    };

    class DTree
    {
    public:
        /// <summary>
        /// the root node of the tree.
        /// read-only please.
        /// </summary>
        UPtr<DTNode> root = nullptr;

        /// <summary>
        /// settings for the system.
        /// modified this before you do anything else.
        /// </summary>
        Settings settings;

        DTree(const Settings & settings_): 
            settings(settings_) {}

        /// <summary>
        /// Learn input patches. Please modified this->settings to match the requirements before calling this function.
        /// Beware that the number of patches input must be larger than
        /// the length of a vectorized patch (patchSize * patchSize) and that length must equal what's in the tree's settings,
        /// otherwise shit happens (part of the regression model will not be invertable, or the size of multiplied matrics is incorrect)
        /// </summary>
        /// <param name="low_reader"></param>
        /// <param name="high_reader"></param>
        void Learn(const Ptr<ImageReader> & low_reader, const Ptr<ImageReader> & high_reader);

        /// <summary>
        /// Predict an image. Expected width and height must be at least the width and height of image 'low'.
        /// </summary>
        /// <param name="low">The input image. Please check if this image is not empty and only with type CV_8U first.</param>
        /// <param name="expected_width">The width of high-res output image. Must be at least low's width.</param>
        /// <param name="expected_height">The height of high-res output image. Must be at least low's height</param>
        /// <param name="fuse_option"></param>
        /// <returns>The high-res image predict by this decision tree</returns>
        Mat PredictImage(const Mat & in_low, cv::Size size) const;

        /// <summary>
        /// Predict an input patch. Please modified this->settings to match requirements before calling this function.
        /// Beware that the size of this two patch must be the same and match settings.patchSize.
        /// </summary>
        /// <param name="in_patch">Input patch for the prediction. Cols and rows must be the same as settings.patchSize.</param>
        /// <param name="out_patch_ptr">Output patch for the prediction. Must be pre-allocated. Cols and rows must be the same as settings.patchSize.</param>
        Mat PredictPatch(const Mat & in_patch) const;

        void PrintBrief(std::ostream & os) const;

        inline int GetNumNodes() const
        {
            return root ? root->GetNumNodes() : 0;
        }

        inline int GetNumLeafNodes() const 
        {
            return root ? root->GetNumLeafNodes() : 0;
        }

    private:
        void Learn(const Ptr<TrainingData> & total_samples);
    };

    class HDTrees
    {
    public:
        vector<DTree> trees;
        Settings settings;

        HDTrees(const Settings & settings_):
            settings(settings_) {}

        void Learn(const Ptr<ImageReader> & low_reader, const Ptr<ImageReader> & high_reader);

        Mat PredictImage(Mat low, cv::Size expected_size) const;
    };
}

// -----------------------------------------
// Some implementation for template function
// -----------------------------------------
