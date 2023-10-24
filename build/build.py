#!/usr/bin/env python3

from dataclasses import dataclass
from functools import cache
from pathlib import Path
from typing import Generator, cast

from bs4 import BeautifulSoup
from bs4.element import NavigableString, Tag
from django.template import Context, Engine

OUR_ROOT = Path(__file__).parents[1].resolve()
TEMPLATES_DIR = OUR_ROOT / 'build' / 'templates'
OUR_ROOT /= 'pages'


@dataclass
class PageProps:
    title: str
    path: Path

    def relative_path(self) -> str:
        return self.path.relative_to(OUR_ROOT).as_posix()


def get_page_props(page_content: str, page_path: Path) -> PageProps:
    soup = BeautifulSoup(page_content, 'html5lib')
    if soup.body is None:
        raise RuntimeError('BeautifulSoup broke')

    children = cast(
        Generator[Tag, None, None],
        (a for a in soup.body.children if type(a) is not NavigableString),
    )
    first_child = next(children)

    if first_child.name == 'h1':
        return PageProps(title=first_child.get_text(), path=page_path)

    raise RuntimeError('Missing title')


@cache
def get_engine():
    return Engine(dirs=[TEMPLATES_DIR.as_posix()])


def render_to_string(template_string: str, context_dict: dict) -> str:
    template = get_engine().from_string(template_string)
    return template.render(Context(context_dict))


def natlab_templates() -> list[PageProps]:
    skip_folders = {
        'shared',
    }

    results: list[PageProps] = []

    for path in OUR_ROOT.iterdir():
        if path.name not in skip_folders and path.is_dir():
            index_template = path / '_index.html'
            if not index_template.is_file():
                continue

            print('*', path.name)

            page = render_to_string(
                index_template.read_text(encoding='utf-8'),
                {
                    'prngs': ['Mulberry32', 'SplitMix32'],
                },
            )
            out_path = path / 'index.html'
            with open(out_path, 'w', encoding='utf-8', newline='\n') as out:
                out.write(page)

            props = get_page_props(page, out_path)
            print(f' {props.title!r}')

            results.append(props)

    return results


def natlab_index(pages: list[PageProps]):
    index_template = TEMPLATES_DIR / 'index.html'
    page = render_to_string(
        index_template.read_text(encoding='utf-8'),
        {
            'pages': pages,
        },
    )
    out_path = OUR_ROOT / 'index.html'
    with open(out_path, 'w', encoding='utf-8', newline='\n') as out:
        out.write(page)


if __name__ == '__main__':
    print('natlab: templates')
    pages = natlab_templates()

    print('natlab: index')
    natlab_index(pages)
