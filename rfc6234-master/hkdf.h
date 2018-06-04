//
//  hkdf.h
//  Workbench
//
//  Created by Wai Man Chan on 17/9/14.
//
//

#ifndef Workbench_hkdf_h
#define Workbench_hkdf_h

#include "../Configuration.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


	int hkdf(const unsigned char *salt, int salt_len,
		const unsigned char *ikm, int ikm_len,
		const unsigned char *info, int info_len,
		uint8_t okm[], int okm_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
