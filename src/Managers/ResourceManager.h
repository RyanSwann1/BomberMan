#pragma once

#include <Utilities\FileDirectory.h>
#include <assert.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <memory>

template <class T>
class ResourceManager
{
	class Resource 
	{
	public:
		Resource(const std::function<const T&(const std::string&)>& fp_loadFromFile, const std::string& fileDirectory)
			: m_resource(&fp_loadFromFile(fileDirectory)),
			m_counter(0)
		{}
		~Resource()
		{
			delete m_resource;
			m_resource = nullptr;
		}
		Resource(const Resource&) = delete;
			
		const T& get()
		{
			++m_counter;
			return *m_resource;
		}

		bool release()
		{
			--m_counter;
			if (!m_counter)
			{
				delete m_resource;
				m_resource = nullptr;
				return true;
			}
			return false;
		}

	private:
		const T* m_resource;
		int m_counter;
	};

public:
	ResourceManager(std::vector<FileDirectory>&& fileDirectories)
	: m_resources(),
	m_fileDirectories(std::move(fileDirectories))
	{
	}

	virtual ~ResourceManager() {}
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager&& operator=(ResourceManager&&) = delete;

	const T& getResource(const std::string& resourceName)
	{
		auto iter = m_resources.find(resourceName);
		if (iter != m_resources.cend())
		{
			return iter->second->get();
		}
		
		addResource(resourceName);

		auto resource = m_resources.find(resourceName);
		assert(resource != m_resources.cend());
		return resource->second->get();
	}

	void releaseResource(const std::string& resourceName)
	{
		auto iter = m_resources.find(resourceName);
		assert(iter != m_resources.cend());
		if (iter->second->release())
		{
			m_resources.erase(iter);
		}
	}

protected:
	virtual const T& loadFromFile(const std::string& fileDirectory) const = 0;

private:
	std::unordered_map<std::string, std::unique_ptr<Resource>> m_resources;
	const std::vector<FileDirectory> m_fileDirectories;

	void addResource(const std::string& resourceName)
	{
		m_resources.emplace(resourceName,
			std::make_unique<Resource>(std::bind(&ResourceManager::loadFromFile, this, std::placeholders::_1), getFileDirectory(resourceName)));
	}

	const std::string& getFileDirectory(const std::string& fileName) const
	{
		auto iter = std::find_if(m_fileDirectories.cbegin(), m_fileDirectories.cend(), [&fileName](const auto& fileDirectory) { return fileDirectory.m_fileName == fileName; });
		assert(iter != m_fileDirectories.cend());
		return iter->m_fileDirectory;
	}
};
