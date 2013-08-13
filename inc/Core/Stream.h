/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Pointers.h"
#include "Core/String.h"
#include <cstdio>

NAMESPACE_CORE_BEGIN

typedef struct zzip_dir ZZIP_DIR;
typedef struct zzip_file ZZIP_FILE;
typedef void CURL;

//-----------------------------------//

/**
 * Use these for different kinds of access to the streams.
 */
enum struct StreamOpenMode
{
	Read,
	Write,
	Append,
	Default
};

/**
 * Controls where the seeking will occur from.
 */
enum struct StreamSeekMode
{
	Absolute,
	Relative,
	RelativeEnd
};

//-----------------------------------//

/**
 * A stream allows uniform access to data backed by different storage
 * mediums like memory, files, archives, or even in remove servers.
 */
class API_CORE Stream
{
public:
	
	enum { EndOfStream = 0, InvalidState = -1 };

protected:

	/**
	 * Creates stream from a path in a given access mode.
	 * \param path stream path
	 * \param mode stream opening mode
	 */
	Stream(const String& path, StreamOpenMode mode);

public:

	/**
	 * Opens the stream.
	 * \return indication wether opening succeeded  
	 */
	virtual bool open();
	
	/**
	 * Closes the stream.
	 * \return indication wether closing was succceeded 
	 */
	virtual bool close();
	
	/**
	 * Reads from the the stream into a buffer.
	 * \param buffer buffer to read into
	 * \param size number of bytes to read
	 * \return number of bytes read 
	 */
	virtual int64 read(void* buffer, uint64 size) const;
	
	/** 
	 * Writes from buffer into the the stream.
	 * \param buffer buffer to write from
	 * \param size number of bytes to write
	 * \return number of bytes written 
	 */
	virtual int64 write(void* buffer, uint64 size);

	/**
	 * Retrieves stream current position.
	 * \return stream position  
	 */
	virtual int64 getPosition() const;

	/**
	 * Set stream position.
	 * \param pos offset position in stream
	 * \param mode set pos offset mode
	 */
	virtual void setPosition(int64 pos, StreamSeekMode mode);

	/**
	 * Get stream size.
	 * \return stream size
	 */
	virtual uint64 size() const;

	/**
	 * Resize stream.
	 * \param size new size
	 */
	virtual void resize(int64 size);

	/** 
	 * Reads from the the stream into a byte vector.
	 * \param data byte vector to read into
	 * \return number of bytes read 
	 */ 
	int64 read(std::vector<uint8>& data) const;

	/** 
	 * Reads from the the stream into a buffer.
	 * \param buffer buffer to read into
	 * \param size number of bytes to read
	 * \return number of bytes read 
	 */ 
	int64 readBuffer(void* buffer, int64 size) const;
	
	/** 
	 * Reads from the the stream into a string.
	 * \param str string to read into
	 * \return string size 
	 */ 
	int64 readString(String& text) const;

	/** 
	 * Reads from the the stream into a list of lines.
	 * \param lines string vector to read into
	 * \return text size 
	 */ 
	int64 readLines(std::vector<String>& lines) const;
	
	/** 
	 * Writes from buffer into the the stream.
	 * \param buffer buffer to write from
	 * \param size number of bytes to write
	 * \return number of bytes written 
	 */
	int64 write(uint8* buf, uint64 size);
	
	/** 
	 * Writes from string into the the stream.
	 * \param string string to write from
	 * \return number of bytes written 
	 */
	int64 writeString(const String& string);

	String path; //!< stream path
	StreamOpenMode mode; //!< stream open mode
};

//-----------------------------------//

class API_CORE FileStream : public Stream
{
public:

	/** 
	 * Opens file by its path in a given access mode.
	 * \param path file path
	 * \param mode file opening mode
	 */
	FileStream(const Path& path, StreamOpenMode mode);

	/**
	 * \note calls \see close()
	 */
	virtual ~FileStream();

	/** 
	 * Opens the stream.
	 * \return indication wether opening succeeded
	 */
	virtual bool open() override;
	
	/** 
	 * Closes the stream.
	 * \return indication wether closing was succceeded 
	 */
	virtual bool close() override;
	
	/** 
	 * Reads from the the stream into a buffer.
	 * \param buffer buffer to read into
	 * \param size number of bytes to read
	 * \return number of bytes read or \see Stream::InvalidState if file handle not set
	 */ 
	virtual int64 read(void* buffer, uint64 size) const override;
	
	using Stream::read;

	/** 
	 * Writes from buffer into the the stream.
	 * \param buffer buffer to write from
	 * \param size number of bytes to write
	 * \return number of bytes written or \see Stream::InvalidState if file handle not set
	 */
	virtual int64 write(void* buffer, uint64 size) override;

	/** 
	 * Retrieves stream current position.
	 * \return stream position or \see Stream::InvalidState if file handle not set  
	 */
	virtual int64 getPosition() const override;

	/** 
	 * Set stream position.
	 * \param pos offset position in stream  
	 * \param mode set pos offset mode
	 */
	virtual void setPosition(int64 pos, StreamSeekMode mode) override;
	
	/** 
	 * Get stream size.
	 * \return stream size  or \see Stream::InvalidState if file handle not set 
	 */
	virtual uint64 size() const override;
	
	/** 
	 * Controls wether IO buffering is active or not.
	 * \param state true to set buffering active, false to do otherwise  
	 */
	void setBuffering(bool state);

	FILE* fileHandle; //!< file handle

	bool isValid; 
};

//-----------------------------------//

class API_CORE MemoryStream : public Stream
{
public:

	/**
	 * Creates Memory stream.
	 */
	MemoryStream();

	/**
	 * Creates Memory stream with an initial size. 
	 * \param size buffer initial size
	 */
	MemoryStream(uint64 size);

	/**
	 * \note calls \see close()
	 */
	virtual ~MemoryStream();

	/** 
	 * Set stream buffer.
	 * \param buffer buffer to use for the stream
	 */
	void setRawBuffer(uint8* buffer);

	/** 
	 * Opens the stream.
	 * \return indication wether opening succeeded  
	 */
	virtual bool open() override;
	
	/** 
	 * Closes the stream.
	 * \return indication wether closing was succceeded 
	 */
	virtual bool close() override;

	/** 
	 * Reads from the the stream into a buffer.
	 * \param buffer buffer to read into
	 * \param size number of bytes to read
	 * \return number of bytes read 
	 */ 
	virtual int64 read(void* buffer, uint64 size) const override;

	/** 
	 * Writes from buffer into the the stream.
	 * \param buffer buffer to write from
	 * \param size number of bytes to write
	 * \return number of bytes written 
	 */
	virtual int64 write(void* buffer, uint64 size) override;

	/** 
	 * Retrieves stream current position.
	 * \return stream position  
	 */
	virtual int64 getPosition() const override;
	
	/** 
	 * Set stream position.
	 * \param pos offset position in stream  
	 * \param mode set pos offset mode
	 */
	virtual void setPosition(int64 pos, StreamSeekMode mode) override;
		
	/** 
	 * Get stream size.
	 * \return stream size  
	 */
	virtual uint64 size() const override;
	
	/** 
	 * Resize stream. 
	 * \param size new size  
	 */
	virtual void resize(int64 size) override;

	/** 
	 * Init memory stream.
	 */
	void init();

public:

	std::vector<uint8> data; //!< buffer vector
	uint8* buffer; //!< pointer to buffer start
	mutable uint64 position; //!< current position in buffer
	bool useRawBuffer; //!< using set buffer or data
};

//-----------------------------------//

class API_CORE WebStream : public Stream
{
public:

	/**
	 * Creates Web stream.
	 * \param URL url webstream is using 
	 * \param mode access mode to url
	 */
	WebStream(const String& URL, StreamOpenMode mode);

	/**
	 * \note calls \see close()
	 */
	virtual ~WebStream();
	
	/** 
	 * Opens the stream.
	 * \return indication wether opening succeeded  
	 */
	virtual bool open() override;
	
	/** 
	 * Closes the stream.
	 * \return indication wether closing was succceeded 
	 */
	virtual bool close() override;
	
	/** 
	 * Reads from the the stream into a buffer.
	 * \param buffer buffer to read into
	 * \param size number of bytes to read
	 * \return number of bytes read 
	 */ 
	virtual int64 read(void* buffer, uint64 size) const override;	
	
	/** 
	 * Writes from buffer into the the stream.
	 * \param buffer buffer to write from
	 * \param size number of bytes to write
	 * \return number of bytes written 
	 */
	virtual int64 write(void* buffer, uint64 size) override;

	/** 
	 * Get stream size.
	 * \return stream size  
	 */
	virtual uint64 size() const override;

	/** 
	 * Update memory stream from current url content.
	 * \return success of update  
	 */
	bool perform() const;

	CURL* handle; //!< curl handle
	MemoryStream ms; //!< memory stream
	bool isPerformDone; //!< wether perform has been run once
};

//-----------------------------------//

class API_CORE ZipStream : public Stream
{
public:

	ZipStream();
	ZipStream(ZZIP_DIR* dir, ZZIP_FILE* handle, String path, StreamOpenMode mode);

public:

	/**
	 * \note calls \see close()
	 */
	virtual ~ZipStream();

	/** 
	 * Opens the stream.
	 * \return indication wether opening succeeded 
	 */
	virtual bool open() override;

	/** 
	 * Closes the stream.
	 * \return indication wether closing was succceeded 
	 */
	virtual bool close() override;

	/** 
	 * Reads from the the stream into a buffer.
	 * \param buffer buffer to read into
	 * \param size number of bytes to read
	 * \return number of bytes read 
	 */ 
	virtual int64 read(void* buffer, uint64 size) const override;
	
	/** 
	 * Retrieves stream current position.
	 * \return stream position  
	 */
	virtual int64 getPosition() const override;

	/** 
	 * Set stream position.
	 * \param pos offset position in stream  
	 * \param mode set pos offset mode
	 */
	virtual void setPosition(int64 pos, StreamSeekMode mode) override;
	
	/** 
	 * Get stream size.
	 * \return stream size  
	 */
	virtual uint64 size() const override;

	friend class ArchiveZip;
	ZZIP_DIR* dir; //!< archive directory
	ZZIP_FILE* handle; //!< archive file handle
};

//-----------------------------------//

NAMESPACE_CORE_END