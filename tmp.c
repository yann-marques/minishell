
int	get_n_tokens(t_token *tokens, int count)
{
	t_token	*tmp;
	int		i;

	tmp = tokens;
	i = 0;
	while (tmp && i < count)
	{
		tmp = tmp->next;
		++i;
	}
	return (tmp);
}

int multi_commands(t_ms *head)
{
    t_token *tmp;

    tmp = get_n_token(head->tokens, head->token_count);
    while (tmp)
    {
        if (tmp->type == _cmd_grp)
            launch_process(head);
		head->token_count += 1;
        tmp = tmp->next;
    }
    return (0);
}