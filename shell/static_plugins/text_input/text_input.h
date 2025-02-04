/*
 * Copyright 2020-2022 Toyota Connected North America
 * @copyright Copyright (c) 2022 Woven Alpha, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <map>
#include <memory>
#include <string>

#include <flutter/binary_messenger.h>
#include <flutter/method_channel.h>
#include <flutter/method_result_functions.h>
#include "flutter/fml/macros.h"
#include "flutter/shell/platform/common/json_method_codec.h"
#include "flutter/shell/platform/common/text_input_model.h"

#include <rapidjson/document.h>
#include <shell/platform/embedder/embedder.h>
#include <xkbcommon/xkbcommon.h>

#include "constants.h"

#if ENABLE_PLUGIN_KEY_EVENT
#include "static_plugins/key_event/key_event.h"
#endif

class Engine;

class TextInput : public flutter::BinaryMessenger {
 public:
  static constexpr char kChannelName[] = "flutter/textinput";

  explicit TextInput();

  /**
   * @brief Callback function for platform messages about textinput
   * @param[in] message Receive message
   * @param[in] userdata Pointer to User data
   * @return void
   * @relation
   * flutter
   */
  static void OnPlatformMessage(const FlutterPlatformMessage* message,
                                void* userdata);

  /**
   * @brief Set the Flutter engine for text input
   * @param[in] engine Pointer to Flutter engine
   * @return void
   * @relation
   * flutter
   */
  void SetEngine(const std::shared_ptr<Engine>& engine);

  /**
   * @brief Update keyboard key status
   * @param[in] data Pointer to User data
   * @param[in] keyboard No use
   * @param[in] serial No use
   * @param[in] time No use
   * @param[in] keysym Keyboard key
   * @param[in] state Keyboard key status
   * @return void
   * @relation
   * flutter
   */
  static void keyboard_handle_key(void* data,
                                  xkb_keysym_t keysym,
                                  uint32_t state);

#if ENABLE_PLUGIN_KEY_EVENT
  static std::shared_ptr<DelegateHandleKey> GetDelegate(
      void* data,
      FlutterKeyEventType type,
      uint32_t xkb_scancode,
      xkb_keysym_t keysym);

  static void handle_key_hook(void* data,
                              FlutterKeyEventType type,
                              uint32_t xkb_scancode,
                              xkb_keysym_t keysym);
#endif

  FML_DISALLOW_COPY_AND_ASSIGN(TextInput);

 private:
  static constexpr char kSetEditingStateMethod[] = "TextInput.setEditingState";
  static constexpr char kClearClientMethod[] = "TextInput.clearClient";
  static constexpr char kSetClientMethod[] = "TextInput.setClient";
  static constexpr char kShowMethod[] = "TextInput.show";
  static constexpr char kHideMethod[] = "TextInput.hide";

  static constexpr char kMultilineInputType[] = "TextInputType.multiline";

  static constexpr char kUpdateEditingStateMethod[] =
      "TextInputClient.updateEditingState";
  static constexpr char kPerformActionMethod[] =
      "TextInputClient.performAction";

  static constexpr char kTextInputAction[] = "inputAction";
  static constexpr char kTextInputType[] = "inputType";
  static constexpr char kTextInputTypeName[] = "name";
  static constexpr char kComposingBaseKey[] = "composingBase";
  static constexpr char kComposingExtentKey[] = "composingExtent";
  static constexpr char kSelectionAffinityKey[] = "selectionAffinity";
  static constexpr char kAffinityDownstream[] = "TextAffinity.downstream";
  static constexpr char kSelectionBaseKey[] = "selectionBase";
  static constexpr char kSelectionExtentKey[] = "selectionExtent";
  static constexpr char kSelectionIsDirectionalKey[] = "selectionIsDirectional";
  static constexpr char kTextKey[] = "text";

  static constexpr char kBadArgumentError[] = "Bad Arguments";
  static constexpr char kInternalConsistencyError[] =
      "Internal Consistency Error";

  // Sends the current state of the given model to the Flutter engine.
  /**
   * @brief Sends the current state of the given model to the Flutter engine
   * @param[in] model Pointer to model
   * @return void
   * @relation
   * flutter
   */
  void SendStateUpdate(const flutter::TextInputModel& model);

  // Sends an action triggered by the Enter key to the Flutter engine.
  /**
   * @brief Sends an action triggered by the Enter key to the Flutter engine
   * @param[in] model Pointer to model
   * @return void
   * @relation
   * flutter
   */
  void EnterPressed(flutter::TextInputModel* model);

  // The MethodChannel used for communication with the Flutter engine.
  std::unique_ptr<flutter::MethodChannel<rapidjson::Document>> channel_;

  // The active client id.
  int client_id_;

  // The active model. nullptr if not set.
  std::unique_ptr<flutter::TextInputModel> active_model_;

  // Keyboard type of the client. See available options:
  // https://docs.flutter.io/flutter/services/TextInputType-class.html
  std::string input_type_;

  // An action requested by the user on the input client. See available options:
  // https://docs.flutter.io/flutter/services/TextInputAction-class.html
  std::string input_action_;

  std::shared_ptr<Engine> engine_;

  MAYBE_UNUSED mutable flutter::BinaryReply last_reply_handler_;
  MAYBE_UNUSED std::string last_message_handler_channel_;
  MAYBE_UNUSED flutter::BinaryMessageHandler last_message_handler_;

  /**
   * @brief Sends a message to the Flutter engine on this channel expecting a
   * reply
   * @param[in] channel This channel
   * @param[in] message Send message
   * @param[in] message_size Send message size
   * @param[in] reply Binary message reply callback
   * @return void
   * @relation
   * flutter
   */
  void Send(const std::string& channel,
            const uint8_t* message,
            size_t message_size,
            flutter::BinaryReply reply) const override;

  /**
   * @brief Registers a handler to be invoked when the Flutter application sends
   * a message to its host platform
   * @param[in] channel This channel
   * @param[in] handler Handler for incoming binary messages from Flutter
   * @return void
   * @relation
   * flutter
   */
  void SetMessageHandler(const std::string& channel,
                         flutter::BinaryMessageHandler handler) override;
};
