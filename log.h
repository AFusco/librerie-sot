/*
 * The MIT License
 *
 *	Copyright (c) 2016 Alessandro Fusco <alessandro.fusco3[at]studio.unibo.it>
 *
 * 	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/*
 * Imposta livelli di log
 * Maggiore il livello, più il log sarà verbose
 */
#define NO_LOG		0x00
#define ERROR_LVL	0x01
#define INFO_LVL	0x02
#define DEBUG_LVL	0x03

#define ERROR_TAG	"\x1B[31m ERROR \033[0m"
#define INFO_TAG	"\x1B[33m  INFO \033[0m"
#define DEBUG_TAG	" DEBUG "

// Di default, effettua il log di tutto
#ifndef LOG_LVL
#define LOG_LVL		DEBUG_LVL
#endif

// Colora automaticamente il messaggio di log a seconda del PID
#ifndef AUTO_COLOR
#define AUTO_COLOR	1
#endif

/*
 * Codici colori ANSI
 */
#define KNRM	"\x1B[0m"
#define KRED	"\x1B[31m"
#define KGRN	"\x1B[32m"
#define KYEL	"\x1B[33m"
#define KBLU	"\x1B[34m"
#define KMAG	"\x1B[35m"
#define KCYN	"\x1B[36m"
#define KWHT	"\x1B[37m"
#define RESET	"\033[0m" // Reset del colore a default

/* Il rosso e il giallo sono esclusi perchè riservati a "ERROR" e "INFO" */
static const char* colors[] = { KNRM, KGRN, KBLU, KMAG, KCYN, KWHT };

/* Imposta colore automaticamente tramite il numero di pid */
#define KAUTO colors[getpid() % 6]

#define NEWLINE     "\n"


/*
 * Formato:
 * [PID] nomefunzione:riga | LOGTAG |  messaggio di log
 */
#define LOG_FMT			"[%d] %s:%d   \t|%s"
#define LOG_ARGS(LOG_TAG)	getpid(),  __FUNCTION__, __LINE__, LOG_TAG

/*
 * Funzione di stampa
 */
#define LOG_FILE stderr
#define PRINTFUNCTION(format, ...)      fprintf(LOG_FILE, format, __VA_ARGS__)

#define LOG(TAG, message, args...) \
	PRINTFUNCTION(LOG_FMT message RESET NEWLINE, LOG_ARGS(TAG), ## args)

#define LOG_COL(TAG, color, message, args...) \
	PRINTFUNCTION("%s" LOG_FMT message RESET NEWLINE, color, LOG_ARGS(TAG), ## args)

#if AUTO_COLOR
#define LOG_C(TAG, message, args...) \
	LOG_COL(TAG, KAUTO, message, ## args)
#else
#define LOG_C(TAG, message, args...) \
	LOG(TAG, message, ## args)
#endif

#if LOG_LVL >= DEBUG_LVL
#define LOG_DBG(message, args...) \
	LOG_C(DEBUG_TAG, message, ## args)
#else
#define LOG_DBG(message, args...)
#endif

#if LOG_LVL >= INFO_LVL
#define LOG_INF(message, args...) \
	LOG_C(INFO_TAG, message, ## args)
#else
#define LOG_INF(message, args...)
#endif

#if LOG_LVL >= ERROR_LVL
#define LOG_ERR(message, args...) \
	LOG_C(ERROR_TAG, message, ## args)
#else
#define LOG_ERR(message, args...)
#endif

#define PERR(message, args...) \
  LOG_C(ERROR_TAG, message ": %s", ## args, strerror(errno))


#endif 
