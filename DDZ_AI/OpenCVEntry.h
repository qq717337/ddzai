#pragma once
#include "stdafx.h"
#include <opencv2\highgui.hpp>
#include<opencv2\photo\photo.hpp>
#include<vector>
#include<string>
#include<map>
class OpenCVEntry
{
private:
	std::wstring m_cardPath;
	std::map<std::string, cv::Mat>image_map;
public:
	OpenCVEntry(const wchar_t* cardImagePath);
	std::string GetCardName(uint8_t cardValue);
	const cv::Mat GetCardImage(std::map<std::string, cv::Mat>& map, uint8_t cardValue) {
		return  map[GetCardName(cardValue)];
	}
	void Show(const cv::String& img1, const cv::String& img2);
	void ShowCard(CardVector  PlayerCards0, CardVector  PlayerCards1, CardVector  PlayerCards2, CardVector  extraCards);
	void Wait(int delay);
	void DestroyAllWindow();
	~OpenCVEntry();
	const cv::Mat GetCardImage(uint8_t cardValue, bool rotate = false);
};

