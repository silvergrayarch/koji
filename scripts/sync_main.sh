# SPDX-License-Identifier: GPL-3.0
# SPDX-FileCopyrightText: 2026 silver_gray
#!/bin/bash

git switch main && git fetch origin && git rebase origin/development && git push --force-with-lease origin main && git switch development
