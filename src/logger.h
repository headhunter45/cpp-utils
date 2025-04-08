/**********************************************************************************************************************
 *
 * @file logger.h
 *
 * @brief Declares the Logger class used as a generic interface to log to logging destinations.
 *
 * Multiple logging destinations can be registered with different thresholds.
 *
 * @copyright Copyright (C) 2023 by Tom Hicks <headhunter3@gmail.com>
 *
 * Licensed under the MIT license.
 *
 **********************************************************************************************************************/
/*
 *  MIT License
 *
 * Copyright (c) 2023 Tom Hicks <headhunter3@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 **********************************************************************************************************************/
#ifndef CPPUtils__logger_h__
#define CPPUtils__logger_h__

#include <exception>
#include <memory>
#include <string>
#include <vector>

namespace CPPUtils {
/// @addtogroup logger Logger
/// @{

/// @brief The Logger class can be used as a general purpose logging system.
/// It allows the user to register one or more destinations for log requests. Each destination can be configured to
/// respond to different levels of messages. Logger also provides some utility methods to log specific debug conditions.
/// LogUnimplementedMethod, LogUnhandledError, LogUnimplementedFeature, and LogToDo.
///
/// * @link LogUnimplementedMethod @endlink logs a debug message that the calling method has not been implemented. When
/// compiled with c++20 or higher it is a normal function, but for earlier c++ versions there is a #define
/// LogUnimplementedMethod that calls @link LogUnimplementedMethodReal @endlink with the calling function name.
///
/// * @link LogUnhandledError @endlink logs a debug message that some exception was caught the calling code doesn't know
/// what to do with it, but wants to try to continue.
///
/// * @link LogUnimplementedFeature @endlink logs a debug message that some feature has not been implemented yet. This
/// is useful for when a branch of code hasn't been implemented but the rest of the function has.
///
/// * @link LogToDo @endlink logs a debug message that something needs to be done.
class Logger {
public:
  /// @brief The type of a message.
  enum MessageType {
    /// The lowest level. Anything that logs with this level is likely a mistake.
    Unknown = 0,
    /// The second lowest level. This message type is debugging information. The convenience methods all log at this
    /// level.
    Debug,
    /// This is the third lowest level. Logs at this level should be useful to an end user. It is good for detailed
    /// diagnostic messages.
    Verbose,
    /// This is the middle level. It is useful for standard logging such as application configuration or state changes.
    Info,
    /// This is the third highest level. It is useful for when something may be wrong, but it likely recoverable or the
    /// action can be retried. This level is also good for things are not an error, but may produce unexpected results.
    Warning,
    /// This is the second highest level. This is for common errors like failed connections and files not being
    /// writable. At this level something is definitely wrong, but may be recoverable.
    Error,
    /// This is the highest level. Conditions that log at this level are almost always fatal. Use it when something
    /// happens that should be impossible. It is useful for when you have unimplemented code paths that are skipped
    /// because data should never let them be called. It can help track down why something else is crashing or incorrect
    /// because of data model changes.
    Wtf,
  };

  /// @brief This is an abstract class used to represent a logging destination.
  /// Users should subclass Destination and implement LogMessage and both LogError methods to create a new logging
  /// destination.
  ///
  /// @see ConsoleLogger for an example destination that logs to cout.
  class Destination {
  public:
    /// @brief Allocates resources used by this destination. This is currently empty.
    Destination();
    /// @brief Releases any resources allocated by this destination. This is currently empty.
    virtual ~Destination();
    /// @brief Logs a string message with a specific type.
    /// @param type The type of message to log.
    /// @param message The message to log.
    virtual void LogMessage(const MessageType &type, const std::string &message) const = 0;
    /// @brief Logs an exception.
    /// @param type The type of message to log the exception as.
    /// @param ex The exception to log.
    virtual void LogError(const MessageType &type, const std::exception &ex) const = 0;
    /// @brief Logs an exception with a message.
    /// @param type The type of message to log.
    /// @param message The message to log.
    /// @param ex The exception to log.
    virtual void LogError(const MessageType &type, const std::string &message, const std::exception &ex) const = 0;
    /// @brief Sets the minimum message type that this logger logs. Requests to log messages lower than this type will
    /// be ignored.
    /// @param type The message type to set.
    virtual void SetMinType(const MessageType &type);
    /// @brief Sets the maximum message type that this logger logs. Requests to log messages higher than this type will
    /// be ignored.
    /// @param type The message type to set.
    virtual void SetMaxType(const MessageType &type);
    /// @brief Gets the maximum message type that this logger logs.
    /// @return The message type.
    virtual MessageType GetMinType() const;
    /// @brief Gets the minimum message type that this logger logs.
    /// @return The message type.
    virtual MessageType GetMaxType() const;

  protected:
    MessageType min_type_;
    MessageType max_type_;
  };

  /// If you want to create an instance of Logger you should use @link GetShared() @endlink or @link GetUnique()
  /// @endlink.
  /// @name Constructors and Destructors
  /// @{

  /// @brief Gets the shared Logger instance. Treat this like a singleton.
  /// @return The same instance each time it is called.
  static std::shared_ptr<Logger> GetShared();

  /// @brief Gets a unique Logger instance. If you want to separate loggers then use this.
  /// @return A different instance each time it is called.
  static std::shared_ptr<Logger> GetUnique();

  /// @brief Frees resources used by the logger. This currently does nothing.
  virtual ~Logger();

protected:
  /// @brief Allocates resources used by the logger. This currently does nothing.
  Logger();

public:
  /// @}

  /// Methods for manipulation the @link Destinations @endlink of this Logger.
  /// @name Destinations
  /// @{

  /// @brief Adds a destination to this logger.
  /// @param destination The destination to add.
  virtual void AddDestination(std::shared_ptr<Logger::Destination> destination);

  /// @brief Removes all destinations from this logger.
  virtual void ClearDestinations();
  /// @}

  /// These methods log debug messages for specific cases.
  /// @name Convenience Methods
  /// @{

// This one is special and annoying because it requires macros until the minimum standard is c++20.
#if __cplusplus >= 202002L
  /// @brief Logs the calling method as not implemented.
  /// @param The source location this was called from.
  void LogUnimplementedMethod(std::source_location = std::source_location::current());
#else
  /// @brief Logs method_name as not implemented. Don't call this directly use LogUnimplementedMethod instead
  /// @param method_name The name of the method that is not implemented.
  void LogUnimplementedMethodReal(std::string method_name);
#endif

  /// @brief Logs an exception as unhandled.
  /// @param ex The exception to log.
  void LogUnhandledError(const std::exception &ex);

  /// @brief Logs a feature as not implemented.
  /// @param feature The name of the unimplemented feature.
  void LogUnimplementedFeature(const std::string &feature);

  /// @brief Logs a TODO message.
  /// @param message The message to log.
  void LogToDo(const std::string &message);
  /// @}

  /// These methods let you pass the @link MessageType @endlink instead of calling the specifically named methods.
  /// @name General Logging Methods
  /// @{

  /// @brief Logs a message with a specific message type.
  /// @param type The type of message to log.
  /// @param message The message to log.
  void Log(const MessageType &type, const std::string &message);

  /// @brief Logs an exception with a specific message type.
  /// @param type The type of message to log.
  /// @param ex The exception to log.
  void Log(const MessageType &type, const std::exception &ex);

  /// @brief Logs an exception with a message as a specific message type.
  /// @param type The type of message to log.
  /// @param message The message to log.
  /// @param ex The exception to log.
  void Log(const MessageType &type, const std::string &message, const std::exception &ex);
  /// @}

  /// These methods log at the @link Debug @endlink level.
  /// @name Debug
  /// @{

  /// @brief Logs a message as a debug.
  /// @param message The message to log.
  void LogDebug(const std::string &message);
  /// @brief Logs an exception as a debug.
  /// @param ex The exception to log.
  void LogDebug(const std::exception &ex);
  /// @brief Logs an exception with a message as a debug.
  /// @param message The message to log.
  /// @param ex The exception to log.
  void LogDebug(const std::string &message, const std::exception &ex);
  /// @}

  /// These methods log at the @link Verbose @endlink level.
  /// @name Verbose
  /// @{

  /// @brief Logs a message as a verbose.
  /// @param message The message to log.
  void LogVerbose(const std::string &message);

  /// @brief Logs an exception as a verbose.
  /// @param ex The exception to log.
  void LogVerbose(const std::exception &ex);

  /// @brief Logs an exception with a message as a verbose.
  /// @param message The message to log.
  /// @param ex The exception to log.
  void LogVerbose(const std::string &message, const std::exception &ex);
  /// @}

  /// These methods log at the @link Info @endlink level.
  /// @name Info
  /// @{

  /// @brief Logs a message as an Info.
  /// @param message The message to log.
  void LogInfo(const std::string &message);

  /// @brief Logs an exception as an Info.
  /// @param ex The exception to log.
  void LogInfo(const std::exception &ex);

  /// @brief Logs an exception with a message as an Info.
  /// @param message The message to log.
  /// @param ex The exception to log.
  void LogInfo(const std::string &message, const std::exception &ex);
  /// @}

  /// These methods log at the @link Warning @endlink level.
  /// @name Warning
  /// @{

  /// @brief Logs a message as a warning.
  /// @param message The message to log.
  void LogWarning(const std::string &message);

  /// @brief Logs an exception as a warning.
  /// @param ex The exception to log.
  void LogWarning(const std::exception &ex);

  /// @brief Logs an exception with a message as a warning.
  /// @param message The message to log.
  /// @param ex The exception to log.
  void LogWarning(const std::string &message, const std::exception &ex);
  /// @}

  /// These methods log at the @link Error @endlink level.
  /// @name Error
  /// @{

  /// @brief Logs a message as an error.
  /// @param message The message to log.
  void LogError(const std::string &message);

  /// @brief Logs an exception as an error.
  /// @param ex The exception to log.
  void LogError(const std::exception &ex);

  /// @brief Logs an exception with a message as an error.
  /// @param message The message to log.
  /// @param ex The exception to log.
  void LogError(const std::string &message, const std::exception &ex);
  /// @}

  /// These methods log at the @link Wtf @endlink level.
  /// @name Wtf (What a Terrible Failure)
  /// @{

  /// @brief Logs a message as a terrible failure. This is something that the user expects to never happen.
  /// @param message The message to log.
  void LogWtf(const std::string &message);
  /// @brief Logs an exception as a terrible failure. This is something that the user expects to never happen.
  /// @param ex The exception to log.
  void LogWtf(const std::exception &ex);
  /// @brief Logs an exception with a message as a terrible failure. This is something that user expects to never
  /// happen.
  /// @param message The message to log.
  /// @param ex The exception to log.
  void LogWtf(const std::string &message, const std::exception &ex);
  /// @}

private:
  std::vector<std::shared_ptr<Logger::Destination>> destinations_;
};

/// @}
} // End namespace CPPUtils

/// @addtogropu Logger
/// @{

#if __cplusplus >= 202002L
#elif defined __GNUC__
/// @brief Logs a debug message saying the calling method is not implemented.
#define LogUnimplementedMethod Logger::LogUnimplementedMethodReal(__PRETTY_FUNCTION__)
#else
/// @brief Logs a debug message saying the calling method is not implemented.
#define LogUnimplementedMethod Logger::LogUnimplementedMethodReal(__FUNCTION__)
#endif

/// @}
#endif // End !defined(CPPUtils__logger_h__)
