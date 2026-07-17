#pragma once

#include "scene/scene.h"
#include<string>
#include<unordered_map>
#include <glm/glm.hpp>
#include <random>
#include<any>

struct DataHolder
{
	// local vars
	// scene management
	Scene* currentScene;
	Scene* nextScene;
	bool queue = false;
	bool queueDelete = true;
	
	// physics framerate
	float physicsTick;
	float physicCap;
	float frameTick;
	int frameCapInt;
	float frameCap;

	// static vars
	static DataHolder god;

	// csv specific
	// Editor
	std::vector<std::vector<int>> layers;
	std::vector<glm::vec4> colorList;
	int layerX = 0;
	int layerY = 0;
	int layerZ = 0;
	int layerElems = 0;

	// Delimiters
	const std::string entityDelimDefault = ",";
	std::string entityDelim = entityDelimDefault;
	const std::string lineDelimDefault = "\n";
	std::string lineDelim = lineDelimDefault;
	const std::string layerDelimDefault = "\n\t";
	std::string layerDelim = layerDelimDefault; // <- "pipe" not "l"

	// Entity Strings
	std::vector<std::string> entityStrings;

	void resize(int x,int y,int z, int elems)
	{
		// vector with 0 values to fill empty bits

		int xMin = std::min(x,layerX);
		int yMin = std::min(y,layerY);
		int zMin = std::min(z, layerZ);
		int elemMin = std::min(elems,layerElems);
		int newSize = x*y;
		std::vector<int> v(newSize,0);

		// add and remove layers
		while (layers.size() != z)
		{
			if (layers.size() < z){layers.push_back(v);}
			else{layers.pop_back();}
		}

		// copy already existing layers
		for (int i = 0; i < zMin; i++) // for every layer
		{
			std::vector<int> newLayer;
			newLayer.resize(newSize,0);
			for (int j = 0; j < yMin; j++) // for every y
			{
				for (int k = 0; k < xMin; k++) // for every x
				{
					newLayer[j*x+k] = layers[i][j*layerX+k];
				}
			}
			layers[i] = newLayer;

			if (elems<layerElems)
			{
				for (int j = 0; j < layers[i].size(); j++)
				{
					if (layers[i][j]>=elems) { layers[i][j]=0; }
				}
			}
		}

		// add and remove layers
		while (layers.size() != z)
		{
			if (layers.size() < z){layers.push_back(v);}
			else{layers.pop_back();}
		}

		std::vector<glm::vec4> newColors;

		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		float randValue; // = dist(rng);

		// using different algorythm because I want random values instead of 0'ed ints
		// - propagate a list of random values before copying over old values
		// - could be optimized to include old values // but that would have higher dev cost from algo
		for (int i = 0; i < z; i++)
		{
			for (int j = 0; j < elems; j++)
			{
				float a;
				if (j == 0 && i > 0)
				{
					a = 0;
				}
				else
				{
					a = 1;
				}

				newColors.emplace_back(dist(rng),dist(rng),dist(rng),a);
			}
		}

		for (int i = 0; i < zMin; i++) // for each layer
		{
			for (int j = 0; j < elemMin; j++)
			{
				newColors[j + i*elems] = colorList[j + i*layerElems];
			}
		}
		colorList = newColors;

		std::vector<std::string> newEntityStrings;
		newEntityStrings.resize(elems * z);

		for (int i = 0; i < zMin; i++)
		{
			for (int j = 0; j < elemMin; j++)
			{
				newEntityStrings[i*elems + j] = entityStrings[j + i*layerElems];
			}
		}
		for (int i = 0; i < z; i++)
		{
			for (int j = 0; j < elems; j++)
			{
				newEntityStrings[i*elems + j] = std::to_string(j);
			}
		}
		entityStrings = newEntityStrings;

		// change meta only after resizing because both metas are needed to resize
		layerX = x;
		layerY = y;
		layerZ = z;
		layerElems = elems;
	}
	static void Resize(int x,int y,int z, int elems){god.resize(x,y,z,elems);}


	std::unordered_map<std::string, std::any> uncategorizedData;

	// struct for trash collection
	struct TrashItem
	{
		void* ptr;
		void (*deleter)(void*);
	};

	// trash collection
	std::vector<TrashItem> trashList;

	// enum for text channels to avoid overlap
	enum TEXTCHANNEL
	{
		MEANMENU,
		EDITOR,
		DELIMSET,

		STRINGEDIT,
		ENTITYEDIT,
	};

	// static funcs
	static void init();

	static void ChangeScene(Scene* s, bool clean = true); // direct way to change scene - clean is for deleting previous scene
	static void SceneQueue(Scene* s, bool clean = true); // safer and delayed way to change scene - clean is for deleting previous scene
	static void SceneUpdate(); // part 2 of sceneQueue ^^^

	void handleScene(float time=0); // scene render and scene handle are called here.
	
	// framerate function
	static void SetPhysicsCap(float cap){god.setPhysicsCap(cap);}
	void setPhysicsCap(float cap);
	static void SetFrameCap(float cap){god.setFrameCap(cap);}
	void setFrameCap(float cap);

	// uncategorized data management
	static void SetUnCatData(std::string key, std::any data) { god.setUnCatData(key, data); };
	void setUnCatData(std::string key, std::any data);
	static bool CheckKeyUnCatData(std::string key) { return god.checkKeyUnCatData(key); };
	bool checkKeyUnCatData(std::string key);
	template<typename T>
	T getUnCatData(std::string key) { return std::any_cast<T>(uncategorizedData[key]); }
	static void DeleteUnCatData(std::string key) { god.deleteUnCatData(key); };
	void deleteUnCatData(std::string key);

	//trash collection functions

	// delayDelete is called manually. It is a safer way to delete things than using delete keyword.
	template<typename T>
	static void DelayDelete(T* ptr){god.delayDelete<T>(ptr);}
	template<typename T>
	void delayDelete(T* ptr)
	{
		trashList.push_back({
			ptr,
			[](void* p)
			{
				delete static_cast<T*>(p);
			}
		});
	}
	// trashEmpty is called after scene handle and render to avoid crashes from things getting destroyed too early.
	void trashEmpty();
};