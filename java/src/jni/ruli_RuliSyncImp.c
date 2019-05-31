
#include <assert.h>
#include <ruli.h>

#include "ruli_RuliSyncImp.h"


const char * const STR_TARGET    = "target";
const char * const STR_PRIORITY  = "priority";
const char * const STR_WEIGHT    = "weight";
const char * const STR_PORT      = "port";
const char * const STR_ADDRESSES = "addresses";


static jobjectArray scan_sync_srv_list(JNIEnv *env, ruli_sync_t *sync_query)
{
  jobjectArray result = 0;
  jclass hashmapClass;
  jmethodID hashmapNew;
  jmethodID hashmapPut;
  jclass integerClass;
  jmethodID integerNew;
  jclass stringClass;

  int srv_list_size;
  const ruli_list_t *srv_list;
  int srv_code;
  int i;

  jstring str_target;
  jstring str_priority;
  jstring str_weight;
  jstring str_port;
  jstring str_addresses;

  assert(sync_query);

  srv_code = ruli_sync_srv_code(sync_query);
  assert(srv_code != RULI_SRV_CODE_VOID);

  if (srv_code)
    return result;

  srv_list = ruli_sync_srv_list(sync_query);
  assert(srv_list);

  srv_list_size = ruli_list_size(srv_list);
  assert(srv_list_size >= 0);
  if (srv_list_size < 1)
    return result;

  hashmapClass = (*env)->FindClass(env, "java/util/HashMap");
  assert(hashmapClass);

  result = (*env)->NewObjectArray(env, srv_list_size, hashmapClass, 0);
  assert(result);

  str_target = (*env)->NewStringUTF(env, STR_TARGET);
  str_priority = (*env)->NewStringUTF(env, STR_PRIORITY);
  str_weight = (*env)->NewStringUTF(env, STR_WEIGHT);
  str_port = (*env)->NewStringUTF(env, STR_PORT);
  str_addresses = (*env)->NewStringUTF(env, STR_ADDRESSES);

  assert(str_target);
  assert(str_priority);
  assert(str_weight);
  assert(str_port);
  assert(str_addresses);

  hashmapNew = (*env)->GetMethodID(env, hashmapClass, "<init>", "()V");
  assert(hashmapNew);
  hashmapPut = (*env)->GetMethodID(env, hashmapClass, "put", 
				   "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
  assert(hashmapPut);

  integerClass = (*env)->FindClass(env, "java/lang/Integer");
  assert(integerClass);
  integerNew = (*env)->GetMethodID(env, integerClass, "<init>", "(I)V");
  assert(integerNew);

  stringClass = (*env)->FindClass(env, "java/lang/String");
  assert(stringClass);

  /*
   * Scan list of SRV records
   */
  for (i = 0; i < srv_list_size; ++i) {
    ruli_srv_entry_t *entry = (ruli_srv_entry_t *) ruli_list_get(srv_list, i);
    ruli_list_t      *addr_list = &entry->addr_list;
    int              addr_list_size = ruli_list_size(addr_list);
    int              j;

    /* create java record */
    jobject hashmap = (*env)->NewObject(env, hashmapClass, hashmapNew);
    assert(hashmap);
    (*env)->SetObjectArrayElement(env, result, i, hashmap);

    /* target host */
    {
      char    txt_dname_buf[RULI_LIMIT_DNAME_TEXT_BUFSZ];
      int     txt_dname_len;
      jstring hostname;

      if (ruli_dname_decode(txt_dname_buf, RULI_LIMIT_DNAME_TEXT_BUFSZ,
                            &txt_dname_len,
                            entry->target, entry->target_len))
        continue;

      hostname = (*env)->NewStringUTF(env, txt_dname_buf);
      assert(hostname);
      (*env)->CallVoidMethod(env, hashmap, hashmapPut, str_target, hostname);
    }

    /* priority, weight, port */
    {
      jobject int_priority = (*env)->NewObject(env, integerClass, integerNew, entry->priority);
      jobject int_weight = (*env)->NewObject(env, integerClass, integerNew, entry->weight);
      jobject int_port = (*env)->NewObject(env, integerClass, integerNew, entry->port);

      assert(int_priority);
      assert(int_weight);
      assert(int_port);

      (*env)->CallVoidMethod(env, hashmap, hashmapPut, str_priority, int_priority);
      (*env)->CallVoidMethod(env, hashmap, hashmapPut, str_weight, int_weight);
      (*env)->CallVoidMethod(env, hashmap, hashmapPut, str_port, int_port);
    }

    /* create addr list and add to hashmap */
    jobjectArray addr_array = (*env)->NewObjectArray(env, addr_list_size, stringClass, 0);
    assert(addr_array);
    (*env)->CallVoidMethod(env, hashmap, hashmapPut, str_addresses, addr_array);

    /*
     * scan record addresses
     */
    for (j = 0; j < addr_list_size; ++j) {
      char buf[40];
      jstring str_addr;
      ruli_addr_t *addr = (ruli_addr_t *) ruli_list_get(addr_list, j);
      int len = ruli_addr_snprint(buf, sizeof(buf), addr);
      assert(len > 0);
      assert(len < sizeof(buf));

      /* add addr to addr_list */
      str_addr = (*env)->NewStringUTF(env, buf);
      assert(str_addr);
      (*env)->SetObjectArrayElement(env, addr_array, j, str_addr);
    }

  } /* scan srv records */

  return result;
}


JNIEXPORT jobjectArray JNICALL Java_ruli_RuliSyncImp_srvQuery
  (JNIEnv *env, jclass obj, jstring service, jstring domain, 
   jint fallback_port, jint options)
{
  const char *c_service;
  const char *c_domain;
  jobjectArray srv_list = 0;
    
  c_service = (*env)->GetStringUTFChars(env, service, 0);
  c_domain = (*env)->GetStringUTFChars(env, domain, 0);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_query(c_service, c_domain,
                                 fallback_port, options);
    if (!sync_query)
      goto exit;

    srv_list = scan_sync_srv_list(env, sync_query);
    
    ruli_sync_delete(sync_query);
  }
  
 exit:
  (*env)->ReleaseStringUTFChars(env, service, c_service);
  (*env)->ReleaseStringUTFChars(env, domain, c_domain);
  
  return srv_list;
}

JNIEXPORT jobjectArray JNICALL Java_ruli_RuliSyncImp_smtpQuery
  (JNIEnv *env, jclass obj, jstring domain, jint options)
{
  const char *c_domain;
  jobjectArray srv_list = 0;
    
  c_domain = (*env)->GetStringUTFChars(env, domain, 0);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_smtp_query(c_domain, options);
    if (!sync_query)
      goto exit;

    srv_list = scan_sync_srv_list(env, sync_query);
    
    ruli_sync_delete(sync_query);
  }
  
 exit:
  (*env)->ReleaseStringUTFChars(env, domain, c_domain);
  
  return srv_list;
}

/*
 * Class:     ruli_RuliSyncImp
 * Method:    httpQuery
 * Signature: (Ljava/lang/String;II)[Ljava/lang/Object;
 */
JNIEXPORT jobjectArray JNICALL Java_ruli_RuliSyncImp_httpQuery
  (JNIEnv *env, jclass obj, jstring domain, jint port, jint options)
{
  const char *c_domain;
  jobjectArray srv_list = 0;
    
  c_domain = (*env)->GetStringUTFChars(env, domain, 0);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_http_query(c_domain, port, options);
    if (!sync_query)
      goto exit;

    srv_list = scan_sync_srv_list(env, sync_query);
    
    ruli_sync_delete(sync_query);
  }
  
 exit:
  (*env)->ReleaseStringUTFChars(env, domain, c_domain);
  
  return srv_list;
}
