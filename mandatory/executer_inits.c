/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_inits.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:09:50 by chorst            #+#    #+#             */
/*   Updated: 2024/08/02 14:22:13 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

t_token	*init_pipe_details(t_token *pipetok)
{
	pipetok->detail.pipe.fdin = -1;
	pipetok->detail.pipe.fdout = -1;
	pipetok->detail.pipe.open_prompt = 0;
	return (pipetok);
}

t_token	*init_redirection_details(t_token *redirectiontoken)
{
	redirectiontoken->detail.redirection.fdin = -1;
	redirectiontoken->detail.redirection.fdout = -1;
	redirectiontoken->detail.redirection.rdrctype = NULL;
	return (redirectiontoken);
}
