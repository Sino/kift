/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphinx.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enunes <eocnunes@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 19:23:36 by enunes            #+#    #+#             */
/*   Updated: 2017/11/30 18:27:40 by enunes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kift.h"

#define EN_US	"./en-us"
#define LANG_MDL "./FINAL/7792.lm"
#define LANG_DIC "./FINAL/7792.dic"

int			is_blank(char *str)
{
	int i;

	i = 0;
	while (str[i] == ' ')
	{
		i++;
	}
	if (str[i] == '\0')
		return (1);
	return (0);
}

char const	*sphinx_continued(FILE *fh, ps_decoder_t *ps)
{
	int				rv;
	int16			buf[512];
	int32			score;
	size_t			nsamp;

	rv = ps_start_utt(ps);
	while (!feof(fh))
	{
		nsamp = fread(buf, 2, 512, fh);
		rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
	}
	rv = ps_end_utt(ps);
	return (ps_get_hyp(ps, &score));
}

char const	*call_to_sphinx(void)
{
	ps_decoder_t	*ps;
	cmd_ln_t		*config;
	FILE			*fh;
	char const		*hyp;

	config = cmd_ln_init(NULL, ps_args(), TRUE,
			"-hmm", EN_US, "-lm", LANG_MDL, "-dict", LANG_DIC, NULL);
	if (config == NULL)
		error("Error in pocket sphinx config.");
	if ((ps = ps_init(config)) == NULL)
		error("Error");
	if ((fh = fopen("src.wav", "rb")) < 0)
		error("Error n pocket sphinx");
	hyp = sphinx_continued(fh, ps);
	printf("Recognized: %s\n", hyp); //remove this printf statement once we know how or where to send off the string 'hyp'
	if (hyp == NULL || is_blank((char*)hyp))
		return (strdup("EMPTY COMMAND"));
	fclose(fh);
	ps_free(ps);
	cmd_ln_free_r(config);
	return (hyp);
}
