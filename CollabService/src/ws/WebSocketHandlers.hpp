#pragma once
#include <uwebsockets/App.h>
#include "../shared/Types.hpp"

void handleMessage(uWS::WebSocket<false, true, PerSocketData>* ws, std::string_view msg);
void handleClose(uWS::WebSocket<false, true, PerSocketData>* ws);