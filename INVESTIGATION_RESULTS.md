# GG-SMART Repository Investigation

## 1) Nested Git Directories

**`find /home/sadegh/mohsen/GG-SMART -name '.git' -not -path '*/.git/*' -print`** found 4 `.git` directories:

| # | Path | Size (du -sb) |
|---|------|--------------|
| 1 | `/home/sadegh/mohsen/GG-SMART/.git` (main) | 4,055,129 bytes |
| 2 | `/home/sadegh/mohsen/GG-SMART/مهندسی/کدهای programme/g-vs-bly-loc1/.pio/libdeps/esp32dev/ESPAsyncWebServer/.git` | 1,372,211 bytes |
| 3 | `/home/sadegh/mohsen/GG-SMART/مهندسی/کدهای programme/g-vs-bly-loc1/.pio/libdeps/esp32dev/AsyncTCP@src-7fb2940bccb78b8d2de6915ae328b7fc/.git` | 59,622 bytes |
| 4 | `/home/sadegh/mohsen/GG-SMART/مهندسی/کدهای programme/.git` | 13,781,259 bytes |

- **3 nested git repos** (excluding the main repo at `.git`)
- The `find -maxdepth 2` search only finds the main `.git` at the root, not the deeply nested `.pio/.git` directories

**Nested .git total size: 15,213,092 bytes** (1,372,211 + 59,622 + 13,781,259)

**Main .git size: 4,055,129 bytes**

**Total .git-inclusive size: 19,268,221 bytes**

---

## 2) Consistent Total Project Size

| Measurement | Bytes | MB |
|-------------|-------|----|
| `sum_file_bytes` (awk over all files) | 31,278,603 | 29.83 MB |
| `du -sb /home/sadegh/mohsen/GG-SMART` | 31,278,603 | — |
| `du -sh /home/sadegh/mohsen/GG-SMART` | 33M | — |
| `du -sb /home/sadegh/mohsen/GG-SMART/.git` | 4,055,129 | 3.87 MB |
| `du -sb /home/sadegh/mohsen/GG-SMART --exclude='.git'` | 12,010,382 | 11.46 MB |

**Verification:** 12,010,382 (files only) + 19,268,221 (all .git dirs) = 31,278,603 ✓

**True total project size: 31,278,603 bytes (29.83 MB)** comprising 1,017 files.

**Files-only size (excluding all .git directories): 12,010,382 bytes**

---

## 3) DU With and Without Nested .git (explaining the gap)

- `du -sb /home/sadegh/mohsen/GG-SMART`: 31,278,603 bytes (includes ALL .git dirs)
- `du -sb /home/sadegh/mohsen/GG-SMART --exclude='.git'`: 12,010,382 bytes (excludes ALL .git dirs at any level)
- The 15 MB discrepancy earlier was caused by 3 nested `.git` directories contributing 15,213,092 bytes combined
- Without any .git: **12,010,382 bytes** (the consistent, true project data size)

---

## 4) .obsidian Tracking Resolution

| Check | Result |
|-------|--------|
| `git ls-files` (index) | **24 entries** tracked |
| `git ls-tree HEAD` | **24 entries** tracked |
| Agreement | ✅ Both match — 24 entries in both index and HEAD |
| `git check-ignore -v .obsidian/app.json .obsidian/plugins/obsidian-git/main.js` | No output (files are tracked, so check-ignore is not applicable) |
| `git status --porcelain --untracked-files=normal .obsidian` | Empty — no uncommitted changes |
| `.gitignore` has `.obsidian/` pattern | Present but irrelevant since files are already tracked |

**Answer: .obsidian is tracked in BOTH the index and HEAD** (24 entries each). The earlier contradiction (ls-files=0 vs ls-tree=24) was likely from different repo states; currently both agree at 24.

---

## 5) .obsidian: Re-committed or Frozen One-Time History?

Git log for `.obsidian`:
```
45f5db2 cron backup: 1405-08-20 15:41:57
8fe6cce cron backup: 2026-11-09 19:19:06
af78981 cron backup: 2026-11-09 19:08:06
3df1538 Untrack cli-rest-mcp data.json (contains API key)
6b40e0f cron backup: 2026-10-05 16:30:59
```

**Answer: The .obsidian assets are being RE-COMMITTED on each run** (cron backup pattern). The "cron backup" commits and the "Untrack cli-rest-mcp data.json (contains API key)" commit demonstrate active, periodic updates. The contents are NOT frozen one-time history — they are maintained through repeated commits, likely via a cron job.

---

## 6) Nested Repo: Gitlink or Fully Ignored?

| Check | Result |
|-------|--------|
| `git ls-files -s | awk '$1=="160000"'` | No output (no gitlinks/submodules) |
| `git status --porcelain --ignored=matching | grep -i 'مهندسی'` | No output (engineering dir not ignored) |

**Answer: The 3 nested git repos are NOT staged as gitlinks (submodules) and are NOT fully ignored.** They are physically nested `.git` directories within the main repo that are tracked as part of the overall filesystem but are not git submodules. Their contents are included in `du` measurements and they contribute 15,213,092 bytes across 3 directories.

---

## Summary of Answers

**(a) How many nested git repos exist and their total size:**
- **3 nested git repos** (outside the main `.git`)
- Sizes: ESPAsyncWebServer/.git = 1,372,211 bytes; AsyncTCP/.git = 59,622 bytes; codes/.git = 13,781,259 bytes
- **Nested total: 15,213,092 bytes**

**(b) The true total project size in bytes:**
- **31,278,603 bytes (29.83 MB)** — includes all files + all .git directories
- Files-only (excluding all .git): **12,010,382 bytes**

**(c) Whether .obsidian is tracked in HEAD and in the index:**
- ✅ **Tracked in index**: 24 entries (`git ls-files`)
- ✅ **Tracked in HEAD**: 24 entries (`git ls-tree HEAD`)
- Both agree consistently

**(d) Whether .obsidian assets are being re-committed on each run or are frozen one-time history:**
- ⚠️ **Re-committed on each run** — evidenced by "cron backup" commits in git log and the "Untrack cli-rest-mcp data.json (contains API key)" commit. The assets are actively maintained, not frozen.