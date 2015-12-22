/*
The MIT License (MIT)

Copyright (c) 2013-2014 winlin

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SRS_APP_FORWARD_HPP
#define SRS_APP_FORWARD_HPP

#include <string>
#include <srs_core.hpp>
#include <srs_app_st.hpp>
#include <srs_app_thread.hpp>

#define SRS_FORWARDER_SLEEP_US (int64_t)(3*1000*1000LL)
#define SYS_MAX_FORWARD_SEND_MSGS 128

class ISrsProtocolReaderWriter;
class SrsSharedPtrMessage;
class SrsOnMetaDataPacket;
class SrsMessageQueue;
class SrsRtmpJitter;
class SrsRtmpClient;
class SrsRequest;
class SrsSource;
class SrsKbps;

/**
* forward the stream to other servers.
*/
// TODO: FIXME: refine the error log, comments it.

class SrsForwarder : public ISrsThreadHandler
{
public:
  SrsForwarder(SrsSource* _source);
  virtual ~SrsForwarder();
  virtual int initialize(SrsRequest* req, std::string ep_forward);
  virtual void set_queue_size(double queue_size);
  virtual int on_publish();
  virtual void on_unpublish();
  virtual int on_meta_data(SrsSharedPtrMessage* metadata);
  virtual int on_audio(SrsSharedPtrMessage* msg);
  virtual int on_video(SrsSharedPtrMessage* msg);
  /* interface ISrsThreadHandler. */
  virtual int cycle();
  
private:
  void close_underlayer_socket();
  void discovery_ep(std::string& server, std::string& port, std::string& tc_url);
  int connect_server(std::string& ep_server, std::string& ep_port);
  int connect_app(std::string ep_server, std::string ep_port);
  int forward();

  /* the ep to forward, server[:port]. */
  std::string _ep_forward;
  SrsRequest* _req;
  int stream_id;
  st_netfd_t stfd;
  SrsThread* pthread;
  SrsSource* source;
  ISrsProtocolReaderWriter* io;
  SrsKbps* kbps;
  SrsRtmpClient* client;
  SrsRtmpJitter* jitter;
  SrsMessageQueue* queue;
  /**
   * cache the sequence header for retry when slave is failed.
   * @see https://github.com/simple-rtmp-server/srs/issues/150
    */
  SrsSharedPtrMessage* sh_audio;
  SrsSharedPtrMessage* sh_video;
};

#endif /* ! SRS_APP_FORWARD_HPP */

