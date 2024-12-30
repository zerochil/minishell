/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 08:59:51 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 01:28:39 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_MANAGEMENT_H
# define MEMORY_MANAGEMENT_H

# include "../base/base.h"
#include <unistd.h>
#include <fcntl.h>



void					error(char *message);
void					report_error(char *message);

void					*safe_malloc(size_t size);

void					*int_to_void(int n);

#endif
