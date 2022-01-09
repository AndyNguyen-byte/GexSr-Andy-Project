#pragma once



#include <map>
#include <memory>
#include <string>
#include <cassert>
#include <stdexcept>



template <typename Resource, typename Identifier>

class ResourceHolder {
public:


	void load(Identifier id, const std::string& filename);



	template <typename Parameter>

	void load(Identifier id, const std::string& filename, const Parameter& secondParam);



	Resource& get(Identifier id);

	const Resource& get(Identifier id) const;



private:

	void insertResource(Identifier id, std::unique_ptr<Resource> resource);



private:



	std::map <Identifier, std::unique_ptr<Resource>>     resourceMap;

};



template <typename Resource, typename Identifier>

void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)

{

	// Insert and check success

	auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));

	assert(inserted.second);

}





template <typename Resource, typename Identifier>

void ResourceHolder<typename Resource, typename Identifier>::load(Identifier id, const std::string& filename)

{

	// Create and load resource

	auto resource = std::make_unique<Resource>();

	if (!resource->loadFromFile(filename))

		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);



	// If loading successful, insert resource to map

	insertResource(id, std::move(resource));

}







template <typename Resource, typename Identifier>

template <typename Parameter>

void ResourceHolder<typename Resource,

	typename Identifier>::load(Identifier id,

		const std::string& filename,

		const Parameter& secondParam)

{

	// Create and load resource

	auto resource = std::make_unique<Resource>();

	if (!resource->loadFromFile(filename, secondParam))

		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);



	// If loading successful, insert resource to map

	insertResource(id, std::move(resource));

}



template <typename Resource, typename Identifier>

Resource& ResourceHolder<typename Resource, typename Identifier>::get(Identifier id)

{

	auto found = resourceMap.find(id);

	assert(found != resourceMap.end());



	return *found->second;

}



template <typename Resource, typename Identifier>

const Resource& ResourceHolder<typename Resource, typename Identifier>::get(Identifier id) const

{

	auto found = resourceMap.find(id);

	assert(found != resourceMap.end());



	return *found->second;

}
