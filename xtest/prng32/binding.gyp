{
  "targets": [
    {
      "target_name": "nativeMulberry32",
      "sources": [
        "nativeMulberry32.cxx"
      ],
      "include_dirs": [
        '<!(node -e "require(\'nan\')")'
      ]
    },
    {
      "target_name": "nativeSplitMix32",
      "sources": [
        "nativeSplitMix32.cxx"
      ],
      "include_dirs": [
        '<!(node -e "require(\'nan\')")'
      ]
    }
  ]
}
