# mini-browser

![Status](https://img.shields.io/badge/Status-Complete-success)
![C++](https://img.shields.io/badge/C++-23-00599C?logo=c%2B%2B&logoColor=white)
![SFML](https://img.shields.io/badge/SFML-3-8BC34A)
![libcurl](https://img.shields.io/badge/libcurl-8%2B-073551)
![Build](https://img.shields.io/badge/Build-Make-1f6feb)
![macOS](https://img.shields.io/badge/macOS-Apple-000000?logo=apple&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green)

Minimal desktop web viewer written in modern C++ using SFML 3 for rendering and libcurl for HTTP. Type a URL, fetch the page, render readable text, and click links to navigate—perfect as a learning project and portfolio piece.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Tech Stack](#tech-stack)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
	- [Prerequisites](#prerequisites)
	- [Install dependencies](#install-dependencies)
	- [Build](#build)
	- [Run](#run)
	- [Run tests](#run-tests)
- [Configuration](#configuration)
- [Usage](#usage)
- [Development Notes](#development-notes)
- [Suggested Future Directions](#suggested-future-directions)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## Overview

mini-browser is a compact, event-driven app that demonstrates:

- Basic HTTP networking with redirects and timeouts
- Naive HTML parsing to extract a title, readable text, and links
- A simple UI with a search bar, scrollable content view, and clickable links
- Clean separation of concerns across core, UI, and orchestration layers

---

## Features

- URL/Search Bar
	- Type a URL and press Enter to load
	- Basic URL normalization (adds https:// when missing)

- Fetching and Parsing
	- HTTP GET via libcurl (redirects, timeouts, custom User-Agent)
	- Strips tags, decodes common HTML entities
	- Preserves newlines for <br>, <p>, and block breaks
	- Extracts anchor links (text + href)

- Content Viewer
	- Word wrapping with preserved line breaks
	- Scroll with mouse wheel
	- Clickable links with underlines and navigation
	- Responsive to window resize

- Tests
	- Minimal custom test harness
	- Unit tests for HTML parser logic

---

## Architecture

```
┌──────────────────────┐       ┌──────────────────────────┐
│      SFML Window     │◄──────┤        UI Layer          │
│  (Render + Events)   │       │  SearchBar, ContentView  │
└──────────┬───────────┘       └──────────┬───────────────┘
           │                              │ events/callbacks
           ▼                              ▼
    ┌──────────────┐              ┌──────────────────────┐
    │  Browser     │─────────────►│   Core (Networking   │
    │ Orchestrator │◄─────────────│   + HTML Parsing)    │
    └──────────────┘              └──────────────────────┘
        updates                             data
```

---

## Tech Stack

- C++ 23 (std::async, ranges-friendly code style)
- SFML 3 (windowing, drawing, input)
- libcurl 8+ (HTTP/HTTPS)
- Makefile (simple build)

---

## Project Structure

```
mini-browser/
├── assets/
│   └── HelveticaNeue.ttc         # Font used by UI (required at runtime)
├── include/
│   ├── browser/
│   │   └── browser.h             # App orchestration
│   ├── core/
│   │   ├── html_parser.h         # ParsedPage, Link structs, parser API
│   │   └── http_client.h         # HttpResult, http_get API
│   └── ui/
│       ├── content_view.h        # Scrollable text + link rendering
│       ├── searchbar.h           # URL input widget
│       └── window.h              # SFML window wrapper/event loop
├── src/
│   ├── browser/browser.cpp       # Wires UI ↔ networking/parser
│   ├── core/
│   │   ├── html_parser.cpp
│   │   └── http_client.cpp
│   ├── ui/
│   │   ├── content_view.cpp
│   │   ├── searchbar.cpp
│   │   └── window.cpp
│   └── main.cpp                  # Entry point
├── test/
│   ├── test.h                    # Minimal test framework
│   ├── test_html_parser.cpp      # Parser unit tests
│   └── test_main.cpp             # Test runner
├── Makefile                      # Build and test targets
├── README.md
└── LICENSE
```

---

## Getting Started

### Prerequisites

- macOS (Apple Silicon or Intel)
- Xcode Command Line Tools (for g++)
- Homebrew

### Install dependencies

```zsh
brew install sfml
# libcurl is available on macOS; if you prefer Homebrew's curl:
# brew install curl
```

### Build

```zsh
make
```

### Run

```zsh
./bin/main
```

### Run tests

```zsh
make test
```

---

## Configuration

- Fonts: The UI expects `assets/HelveticaNeue.ttc` to exist. Replace with a preferred font by updating the font load paths in the UI components if desired.
- SFML Location: The Makefile links against Homebrew’s SFML at `/opt/homebrew/opt/sfml`. If SFML is elsewhere, update `CXXFLAGS` and `LDFLAGS` accordingly.

---

## Usage

1. Launch the app: `./bin/main`
2. Click the search bar, type a URL (e.g., `example.com`), and press Enter
3. Read the parsed text; scroll with the mouse wheel
4. Click underlined links to navigate
5. Resize the window—the content view adapts

Notes:
- If a URL is entered without a scheme, `https://` is assumed
- Only basic HTML is supported; complex layouts/scripts are not rendered

---

## Development Notes

- SFML 3 API: Uses the newer event accessors and updated shapes/rects
- Networking: Blocking fetch behind `std::async` for simplicity
- HTML Parsing: Naive text extraction and link finding; suitable for demos
- Tests: Core-only tests avoid SFML to keep runs fast and portable

---

## Limitations

- **No JavaScript**: Scripts are stripped but not executed
- **No CSS**: Styling, colors, fonts, and layouts are ignored
- **No Images**: Image tags are removed; only text content is displayed
- **Basic HTML Support**: Complex nested structures may render incorrectly
- **Single-threaded UI**: Window may freeze briefly during page fetches
- **No HTTPS Verification UI**: Certificate errors are not surfaced to the user
- **Limited Entity Decoding**: Only common entities (&lt;, &gt;, &amp;, &quot;, &#39;) are supported
- **Relative URL Resolution**: Simple logic that may fail for edge cases
- **No History/Bookmarks**: Navigation state is not persisted between sessions
- **macOS-focused**: Font paths and build instructions target macOS (adaptable to Linux/Windows)

---

## Suggested Future Directions

- Loading indicator and status messages in the UI
- Smarter URL normalization and history/back/forward
- Multi-line link rendering improvements
- Simple caching layer for fetched pages
- Replace naive parser with a robust HTML parser (e.g., Gumbo or libxml2)
- Certificate/SSL controls and better error surfacing
- Cross-platform build docs for Linux/Windows

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- [SFML](https://www.sfml-dev.org/) – Simple and fast multimedia library
- [libcurl](https://curl.se/libcurl/) – Multiprotocol file transfer library