#ifndef SWGANH_OBJECT_OBJECT_CONTROLLER_H_
#define SWGANH_OBJECT_OBJECT_CONTROLLER_H_

#include "swganh/object/object_controller.h"
#include "swganh/messages/out_of_band.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

struct ProsePackageWrapper : ProsePackage, wrapper<ProsePackage>
{
    ProsePackageWrapper() : ProsePackage(){}
};

//struct OutOfBandWrapper : OutOfBand, wrapper<OutOfBand>
//{
//    explicit OutOfBandWrapper(const OutOfBandWrapper& wrapper) : OutOfBand() {}
//    OutOfBandWrapper(std::string base_stf_file, std::string base_stf_string, ProseType prose_type, uint64_t object_id) 
//        : OutOfBand(base_stf_file, base_stf_string, prose_type, object_id) {}
//};

void exportOutOfBand()
{
    enum_<ProseType>("ProseType")
        .value("TU", TU)
        .value("TT", TT)
        .value("TO", TO)
        .value("DI", DI)
        .value("DF", DF)
        ;
    class_<OutOfBand, boost::noncopyable>("OutOfBand","object that is used in the help with sending system stf messages")
        .def(init<std::string, std::string, ProseType, uint64_t>())
    ;
}

struct ObjectControllerWrapper : ObjectController, wrapper<ObjectController>
{
};

void exportObjectController()
{
    bool (ObjectController::*SendSystemMessageStr)(const std::string&) = &ObjectController::SendSystemMessage;
    bool (ObjectController::*SendSystemMessageOutOfBand)(const OutOfBand&, bool, bool) = &ObjectController::SendSystemMessage;
    bool (ObjectController::*SendSystemMessageString)(const std::wstring&, bool, bool) = &ObjectController::SendSystemMessage;

    class_<ObjectControllerWrapper, std::shared_ptr<ObjectController>, boost::noncopyable>("ObjectController", no_init)
        .def("SendSystemMessage", SendSystemMessageOutOfBand, "Sends the specified system message to the player with an out of band object attached")
        .def("SendSystemMessage", SendSystemMessageString, "Sends System Message to the player, taking a string as the message and boolean for chatbox only and another boolean to send to in range")
        .def("SendSystemMessage", SendSystemMessageStr, "Sends System Message to the player, taking a string as the message")
        .def("Notify", &ObjectControllerWrapper::Notify, "Notifies the controller whent he object has been updated")
    ;
}

#endif //  SWGANH_OBJECT_OBJECT_CONTROLLER_H_