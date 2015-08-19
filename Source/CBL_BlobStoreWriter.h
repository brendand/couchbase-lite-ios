//
//  CBL_BlobStoreWriter.h
//  CouchbaseLite
//
//  Created by Jens Alfke on 8/19/15.
//  Copyright © 2015 Couchbase, Inc. All rights reserved.
//

#import "CBL_BlobStore.h"


/** Lets you stream a large attachment to a CBL_BlobStore asynchronously, e.g. from a network download. */
@interface CBL_BlobStoreWriter : NSObject

- (instancetype) initWithStore: (CBL_BlobStore*)store;

@property (copy) NSString* name;

/** Appends data to the blob. Call this when new data is available. */
- (void) appendData: (NSData*)data;

- (void) closeFile;     /**< Closes the temporary file; it can be reopened later. */
- (BOOL) openFile;      /**< Reopens the temporary file for further appends. */
- (void) reset;         /**< Clears the temporary file to 0 bytes (must be open.) */

/** Call this after all the data has been added. */
- (void) finish;

/** Call this to cancel before finishing the data. */
- (void) cancel;

/** Installs a finished blob into the store. */
- (BOOL) install;

/** The number of bytes in the blob. */
@property (readonly) UInt64 length;

/** The contents of the blob. */
@property (readonly) NSData* blobData;

/** After finishing, this is the key for looking up the blob through the CBL_BlobStore. */
@property (readonly) CBLBlobKey blobKey;

/** After finishing, this is the MD5 digest of the blob, in base64 with an "md5-" prefix.
    (This is useful for compatibility with CouchDB, which stores MD5 digests of attachments.) */
@property (readonly) NSString* MD5DigestString;
@property (readonly) NSString* SHA1DigestString;

/** Returns YES if the blob's digest matches the digest string (can be "sha1-" or "md5-"),
    or if the digestString is nil. */
- (BOOL) verifyDigest: (NSString*)digestString;

/** The location of the temporary file containing the attachment contents.
    Will be nil after -install is called. */
@property (readonly) NSString* filePath;

/** A stream for reading the completed blob. */
- (NSInputStream*) blobInputStream;

@end
