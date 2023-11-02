#ifndef VIN_ENGINE_MSGBOX_H
#define VIN_ENGINE_MSGBOX_H

enum class MessageType {
    Info,
    Warning,
    Error,
    Question
};

enum class MessageButton {
    Ok,
    OkCancel,
    YesNo,
    Quit
};

enum class MessageResult {
    None,
    Error,
    Ok,
    Cancel,
    Yes,
    No,
    Close
};

#ifdef VIN_LINUX

#include <gtk/gtk.h>

inline GtkMessageType GetGtkMessageType(MessageType type) {
    switch (type) {
        case MessageType::Info:
            return GTK_MESSAGE_INFO;
        case MessageType::Warning:
            return GTK_MESSAGE_WARNING;
        case MessageType::Error:
            return GTK_MESSAGE_ERROR;
        case MessageType::Question:
            return GTK_MESSAGE_QUESTION;
        default:
            return GTK_MESSAGE_INFO;
    }
}

inline GtkButtonsType GetGtkButtonType(MessageButton button) {
    switch (button) {
        case MessageButton::Ok:
            return GTK_BUTTONS_OK;
        case MessageButton::OkCancel:
            return GTK_BUTTONS_OK_CANCEL;
        case MessageButton::YesNo:
            return GTK_BUTTONS_YES_NO;
        case MessageButton::Quit:
            return GTK_BUTTONS_CLOSE;
        default:
            return GTK_BUTTONS_OK;
    }
}

inline MessageResult GetMessageResult(gint result) {
    switch (result) {
        case GTK_RESPONSE_OK:
            return MessageResult::Ok;
        case GTK_RESPONSE_CANCEL:
            return MessageResult::Cancel;
        case GTK_RESPONSE_YES:
            return MessageResult::Yes;
        case GTK_RESPONSE_NO:
            return MessageResult::No;
        case GTK_RESPONSE_CLOSE:
            return MessageResult::Close;
        default:
            return MessageResult::None;
    }
}

inline MessageResult Show(const char* msg, const char* title, MessageType type, MessageButton button) {
    if (!gtk_init_check(0, nullptr))
        return MessageResult::Error;

    GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GetGtkMessageType(type), GetGtkButtonType(button), msg);
    MessageResult result = GetMessageResult(gtk_dialog_run(GTK_DIALOG(dialog)));
    gtk_widget_destroy(dialog);
    while (g_main_context_iteration(nullptr, false));

    return result;
}

#endif

#endif //VIN_ENGINE_MSGBOX_H