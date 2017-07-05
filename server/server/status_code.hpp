#ifndef SIMPLE_WEB_SERVER_STATUS_CODE_HPP
#define SIMPLE_WEB_SERVER_STATUS_CODE_HPP

#include <string>
#include <vector>

namespace SimpleWeb {
  enum class StatusCode {
    unknown = 0,
    information_continue = 100,
    information_switching_protocols,
    information_processing,
    success_ok = 200,
    success_created,
    success_accepted,
    success_non_authoritative_information,
    success_no_content,
    success_reset_content,
    success_partial_content,
    success_multi_status,
    success_already_reported,
    success_im_used = 226,
    redirection_multiple_choices = 300,
    redirection_moved_permanently,
    redirection_found,
    redirection_see_other,
    redirection_not_modified,
    redirection_use_proxy,
    redirection_switch_proxy,
    redirection_temporary_redirect,
    redirection_permanent_redirect,
    client_error_bad_request = 400,
    client_error_unauthorized,
    client_error_payment_required,
    client_error_forbidden,
    client_error_not_found,
    client_error_method_not_allowed,
    client_error_not_acceptable,
    client_error_proxy_authentication_required,
    client_error_request_timeout,
    client_error_conflict,
    client_error_gone,
    client_error_length_required,
    client_error_precondition_failed,
    client_error_payload_too_large,
    client_error_uri_too_long,
    client_error_unsupported_media_type,
    client_error_range_not_satisfiable,
    client_error_expectation_failed,
    client_error_im_a_teapot,
    client_error_misdirection_required = 421,
    client_error_unprocessable_entity,
    client_error_locked,
    client_error_failed_dependency,
    client_error_upgrade_required = 426,
    client_error_precondition_required = 428,
    client_error_too_many_requests,
    client_error_request_header_fields_too_large = 431,
    client_error_unavailable_for_legal_reasons = 451,
    server_error_internal_server_error = 500,
    server_error_not_implemented,
    server_error_bad_gateway,
    server_error_service_unavailable,
    server_error_gateway_timeout,
    server_error_http_version_not_supported,
    server_error_variant_also_negotiates,
    server_error_insufficient_storage,
    server_error_loop_detected,
    server_error_not_extended = 510,
    server_error_network_authentication_required
  };

  inline const static std::vector<std::pair<StatusCode, std::string>> &status_codes() {
    static std::vector<std::pair<StatusCode, std::string>> status_codes;
	std::pair<StatusCode, std::string> p1(StatusCode::unknown, "");
	std::pair<StatusCode, std::string> p2(StatusCode::information_continue, "100 Continue");
	std::pair<StatusCode, std::string> p3(StatusCode::information_switching_protocols, "101 Switching Protocols");
	std::pair<StatusCode, std::string> p4(StatusCode::information_processing, "102 Processing");
	std::pair<StatusCode, std::string> p5(StatusCode::success_ok, "200 OK");
	std::pair<StatusCode, std::string> p6(StatusCode::success_created, "201 Created");
	std::pair<StatusCode, std::string> p7(StatusCode::success_accepted, "202 Accepted");
	std::pair<StatusCode, std::string> p8(StatusCode::success_non_authoritative_information, "203 Non-Authoritative Information");
	std::pair<StatusCode, std::string> p9(StatusCode::success_no_content, "204 No Content");
	std::pair<StatusCode, std::string> p10(StatusCode::success_reset_content, "205 Reset Content");
	std::pair<StatusCode, std::string> p11(StatusCode::success_partial_content, "206 Partial Content");
	std::pair<StatusCode, std::string> p12(StatusCode::success_multi_status, "207 Multi-Status");
	std::pair<StatusCode, std::string> p13(StatusCode::success_already_reported, "208 Already Reported");
	std::pair<StatusCode, std::string> p14(StatusCode::success_im_used, "226 IM Used");
	std::pair<StatusCode, std::string> p15(StatusCode::redirection_multiple_choices, "300 Multiple Choices");
	std::pair<StatusCode, std::string> p16(StatusCode::redirection_moved_permanently, "301 Moved Permanently");
	std::pair<StatusCode, std::string> p17(StatusCode::redirection_found, "302 Found");
	std::pair<StatusCode, std::string> p18(StatusCode::redirection_see_other, "303 See Other");
	std::pair<StatusCode, std::string> p19(StatusCode::redirection_not_modified, "304 Not Modified");
	std::pair<StatusCode, std::string> p20(StatusCode::redirection_use_proxy, "305 Use Proxy");
	std::pair<StatusCode, std::string> p21(StatusCode::redirection_switch_proxy, "306 Switch Proxy");
	std::pair<StatusCode, std::string> p22(StatusCode::redirection_temporary_redirect, "307 Temporary Redirect");
	std::pair<StatusCode, std::string> p23(StatusCode::redirection_permanent_redirect, "308 Permanent Redirect");
	std::pair<StatusCode, std::string> p24(StatusCode::client_error_bad_request, "400 Bad Request");
	std::pair<StatusCode, std::string> p25(StatusCode::client_error_unauthorized, "401 Unauthorized");
	std::pair<StatusCode, std::string> p26(StatusCode::client_error_payment_required, "402 Payment Required");
	std::pair<StatusCode, std::string> p27(StatusCode::client_error_forbidden, "403 Forbidden");
	std::pair<StatusCode, std::string> p28(StatusCode::client_error_not_found, "404 Not Found");
	std::pair<StatusCode, std::string> p29(StatusCode::client_error_method_not_allowed, "405 Method Not Allowed");
	std::pair<StatusCode, std::string> p30(StatusCode::client_error_not_acceptable, "406 Not Acceptable");
	std::pair<StatusCode, std::string> p31(StatusCode::client_error_proxy_authentication_required, "407 Proxy Authentication Required");
	std::pair<StatusCode, std::string> p32(StatusCode::client_error_request_timeout, "408 Request Timeout");
	std::pair<StatusCode, std::string> p34(StatusCode::client_error_conflict, "409 Conflict");
	std::pair<StatusCode, std::string> p35(StatusCode::client_error_gone, "410 Gone");
	std::pair<StatusCode, std::string> p36(StatusCode::client_error_length_required, "411 Length Required");
	std::pair<StatusCode, std::string> p37(StatusCode::client_error_precondition_failed, "412 Precondition Failed");
	std::pair<StatusCode, std::string> p38(StatusCode::client_error_payload_too_large, "413 Payload Too Large");
	std::pair<StatusCode, std::string> p39(StatusCode::client_error_uri_too_long, "414 URI Too Long");
	std::pair<StatusCode, std::string> p40(StatusCode::client_error_unsupported_media_type, "415 Unsupported Media Type");
	std::pair<StatusCode, std::string> p41(StatusCode::client_error_range_not_satisfiable, "416 Range Not Satisfiable");
	std::pair<StatusCode, std::string> p42(StatusCode::client_error_expectation_failed, "417 Expectation Failed");
	std::pair<StatusCode, std::string> p43(StatusCode::client_error_im_a_teapot, "418 I'm a teapot");
	std::pair<StatusCode, std::string> p44(StatusCode::client_error_misdirection_required, "421 Misdirected Request");
	std::pair<StatusCode, std::string> p45(StatusCode::client_error_unprocessable_entity, "422 Unprocessable Entity");
	std::pair<StatusCode, std::string> p46(StatusCode::client_error_locked, "423 Locked");
	std::pair<StatusCode, std::string> p47(StatusCode::client_error_failed_dependency, "424 Failed Dependency");
	std::pair<StatusCode, std::string> p48(StatusCode::client_error_upgrade_required, "426 Upgrade Required");
	std::pair<StatusCode, std::string> p49(StatusCode::client_error_precondition_required, "428 Precondition Required");
	std::pair<StatusCode, std::string> p50(StatusCode::client_error_too_many_requests, "429 Too Many Requests");
	std::pair<StatusCode, std::string> p51(StatusCode::client_error_request_header_fields_too_large, "431 Request Header Fields Too Large");
	std::pair<StatusCode, std::string> p52(StatusCode::client_error_unavailable_for_legal_reasons, "451 Unavailable For Legal Reasons");
	std::pair<StatusCode, std::string> p53(StatusCode::server_error_internal_server_error, "500 Internal Server Error");
	std::pair<StatusCode, std::string> p54(StatusCode::server_error_not_implemented, "501 Not Implemented");
	std::pair<StatusCode, std::string> p55(StatusCode::server_error_bad_gateway, "502 Bad Gateway");
	std::pair<StatusCode, std::string> p56(StatusCode::server_error_service_unavailable, "503 Service Unavailable");
	std::pair<StatusCode, std::string> p57(StatusCode::server_error_gateway_timeout, "504 Gateway Timeout");
	std::pair<StatusCode, std::string> p58(StatusCode::server_error_http_version_not_supported, "505 HTTP Version Not Supported");
	std::pair<StatusCode, std::string> p59(StatusCode::server_error_variant_also_negotiates, "506 Variant Also Negotiates");
	std::pair<StatusCode, std::string> p60(StatusCode::server_error_insufficient_storage, "507 Insufficient Storage");
	std::pair<StatusCode, std::string> p61(StatusCode::server_error_loop_detected, "508 Loop Detected");
	std::pair<StatusCode, std::string> p62(StatusCode::server_error_not_extended, "510 Not Extended");
	std::pair<StatusCode, std::string> p63(StatusCode::server_error_network_authentication_required, "511 Network Authentication Required");
	status_codes.push_back(p1);
	status_codes.push_back(p2);
	status_codes.push_back(p3);
	status_codes.push_back(p4);
	status_codes.push_back(p5);
	status_codes.push_back(p6);
	status_codes.push_back(p7);
	status_codes.push_back(p8);
	status_codes.push_back(p9);
	status_codes.push_back(p10);
	status_codes.push_back(p11);
	status_codes.push_back(p12);
	status_codes.push_back(p13);
	status_codes.push_back(p14);
	status_codes.push_back(p15);
	status_codes.push_back(p16);
	status_codes.push_back(p17);
	status_codes.push_back(p18);
	status_codes.push_back(p19);
	status_codes.push_back(p20);
	status_codes.push_back(p21);
	status_codes.push_back(p22);
	status_codes.push_back(p23);
	status_codes.push_back(p24);
	status_codes.push_back(p25);
	status_codes.push_back(p26);
	status_codes.push_back(p27);
	status_codes.push_back(p28);
	status_codes.push_back(p29);
	status_codes.push_back(p30);
	status_codes.push_back(p31);
	status_codes.push_back(p32);
	status_codes.push_back(p34);
	status_codes.push_back(p35);
	status_codes.push_back(p36);
	status_codes.push_back(p37);
	status_codes.push_back(p38);
	status_codes.push_back(p39);
	status_codes.push_back(p40);
	status_codes.push_back(p41);
	status_codes.push_back(p42);
	status_codes.push_back(p43);
	status_codes.push_back(p44);
	status_codes.push_back(p45);
	status_codes.push_back(p46);
	status_codes.push_back(p47);
	status_codes.push_back(p48);
	status_codes.push_back(p49);
	status_codes.push_back(p50);
	status_codes.push_back(p51);
	status_codes.push_back(p52);
	status_codes.push_back(p53);
	status_codes.push_back(p54);
	status_codes.push_back(p55);
	status_codes.push_back(p56);
	status_codes.push_back(p57);
	status_codes.push_back(p58);
	status_codes.push_back(p59);
	status_codes.push_back(p60);
	status_codes.push_back(p61);
	status_codes.push_back(p62);
	status_codes.push_back(p63);
	return status_codes;
  }

  inline StatusCode status_code(const std::string &status_code_str) {
    for(auto &status_code : status_codes()) {
      if(status_code.second == status_code_str)
        return status_code.first;
    }
    return StatusCode::unknown;
  }

  inline const std::string &status_code(StatusCode status_code_enum) {
    for(auto &status_code : status_codes()) {
      if(status_code.first == status_code_enum)
        return status_code.second;
    }
    return status_codes()[0].second;
  }
} // namespace SimpleWeb

#endif // SIMPLE_WEB_SERVER_STATUS_CODE_HPP
