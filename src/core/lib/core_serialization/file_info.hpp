
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//  file_info.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Copyright (c) Wargaming.net. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef FILE_INFO_H_
#define FILE_INFO_H_

#pragma once

namespace wgt
{
class FileInfo : public IFileInfo
{
public:
	FileInfo()
		: size_(0)
		, created_(0)
		, modified_(0)
		, accessed_(0)
		, fullPath_("")
		, attributes_(FileAttributes::None)
	{
	}

	FileInfo(uint64_t size, uint64_t created, uint64_t modified, uint64_t accessed,
		const std::string& fullPath, FileAttributes::FileAttribute attributes)
		: size_(size)
		, created_(created)
		, modified_(modified)
		, accessed_(accessed)
		, fullPath_(fullPath)
		, attributes_(attributes)
	{
	}

	bool isDirectory() const override
	{
		return ( attributes_ & FileAttributes::Directory ) == FileAttributes::Directory;
	}

	bool isReadOnly() const override
	{
		return ( attributes_ & FileAttributes::ReadOnly ) == FileAttributes::ReadOnly;
	}

	bool isHidden() const override
	{
		return ( attributes_ & FileAttributes::Hidden ) == FileAttributes::Hidden;
	}

	bool isDots() const override
	{
		return isDirectory() && fullPath_.length() > 0 && fullPath_.back() == '.';
	}

	const char* name() const override
	{
		auto index = fullPath_.find_last_of("/\\");
		return &fullPath_.c_str()[index != std::string::npos ? index + 1 : 0];
	}

	const char* extension() const override
	{
		auto index = fullPath_.rfind(FilePath::kExtensionSeparator);
		return index != std::string::npos ? &fullPath_.c_str()[index + 1] : "";
	}

	const char* fullPath() const override
	{
		return fullPath_.c_str();
	}

	uint64_t size() const override { return size_; }
	uint64_t created() const override { return created_; }
	uint64_t modified() const override { return modified_; }
	uint64_t accessed() const override { return accessed_; }

	const FileAttributes::FileAttribute	attributes() const override { return attributes_; }

private:
	const uint64_t			size_;
	const uint64_t			created_;
	const uint64_t			modified_;
	const uint64_t			accessed_;
	const std::string		fullPath_;

	const FileAttributes::FileAttribute	attributes_;
};

}
#endif // FILE_INFO_H_
